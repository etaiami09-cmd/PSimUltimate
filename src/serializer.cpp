#include <vector>
#include <string>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <unordered_map>

#include "Particle.hpp"
#include "json.hpp"
#include "particles.hpp"
#include "constants.hpp"
#include "attributes.hpp"

#include "serializer.hpp"

#include "module.hpp"
#include "pop_up_alerts.hpp"

namespace {

std::map<std::string, std::function<std::string()>> moduleSerializers;
std::map<std::string, std::function<void(std::string)>> moduleDeserializers;

std::unordered_map<std::string, float> buildConstantsMap() {
    std::unordered_map<std::string, float> map;
    for (const auto& constant : getAllConstants()) {
        map[constant.first] = constant.second.value;
    }
    return map;
}

void serializeModules(nlohmann::json& json) {
    nlohmann::json modules;
    auto moduleIndexIterator = getModuleIndexOrders().begin();
    for (size_t i = 0; i < getModuleIndexOrders().size(); i++) {
        modules[getModules()[*moduleIndexIterator].name] = i;
        ++moduleIndexIterator;
    }
    json["Modules"] = modules;
	nlohmann::json moduleData;
	for (const auto& moduleSerializer : moduleSerializers) {
		moduleData[moduleSerializer.first] = moduleSerializer.second();
	}
	json["Module-Specific Data"] = moduleData;
}

void serializeConstants(nlohmann::json& json) {
    json["Constants"] = buildConstantsMap();
}

void serializeParticle(nlohmann::json& json, size_t index) {
    auto mass = Particles::get()[index].getMass();
    auto position = Particles::get()[index].getPosition();
    auto velocity = Particles::get()[index].getVelocity();
    auto radius = Particles::get()[index].getRadius();
    nlohmann::json particle;
    particle["Mass"] = mass;
    particle["Position"] = {position.x, position.y};
    particle["Velocity"] = {velocity.x, velocity.y};
    particle["Radius"] = radius;
    for (const auto& attribute : getAttributes()) {
        particle[attribute.name] = attribute.values[index];
    }
    json["Particles"].push_back(particle);
}

void deserializeConstants(nlohmann::json& json) {
    nlohmann::json constants = json["Constants"];
    for (auto& constant : getAllConstants()) {
        if (!constants[constant.first].empty()) {
            auto value = constants[constant.first].get<float>();
            constant.second.value = value;
            constant.second.buffer = value;
            constant.second.onChange(value);
        }
    }
}
void deserializeParticle(size_t index, nlohmann::json& particles) {
    auto mass = particles[index]["Mass"].get<float>();
    auto radius = particles[index]["Radius"].get<float>();
    auto position = Position{
        particles[index]["Position"][0].get<float>(),
        particles[index]["Position"][1].get<float>()};
    auto velocity = Velocity{
        particles[index]["Velocity"][0].get<float>(),
        particles[index]["Velocity"][1].get<float>()
    };
    std::vector<float> attributeValues;
    attributeValues.clear();
    attributeValues.reserve(getAttributes().size());
    for (const auto& attribute : getAttributes()) {
        attributeValues.push_back(particles[index][attribute.name].get<float>());
    }
    setParticleAttributes(index, attributeValues);
    Particles::set(index, position, velocity, radius, mass);
}

void deserializeModules(nlohmann::json& modules) {
    auto map = modules.get<nlohmann::json::object_t>();
    std::map<std::string, size_t> rawMap;
    std::ranges::transform(map, std::inserter(rawMap, rawMap.end()),
                           [](const auto& pair) {
                               return std::pair{pair.first, pair.second.template get<size_t>()};
                           });
    std::vector<std::pair<std::string, size_t>> mapVector{rawMap.begin(), rawMap.end()};
    std::ranges::sort(mapVector, {}, &std::pair<std::string, size_t>::second);
    auto orderedModuleNames = std::ranges::to<std::vector<std::string>>(
        mapVector | std::views::transform(&std::pair<std::string, size_t>::first));
    setModuleOrders(orderedModuleNames);
}

void deserializeModuleData(nlohmann::json& moduleData) {
	for (const auto& moduleDeserializer : moduleDeserializers) {
		moduleDeserializer.second(moduleData[moduleDeserializer.first]);
	}
}

bool assertValidModules(const nlohmann::json& modules) {
    if (!modules.is_object()) {
        pushPopUpAlert("Deserialization error: Modules is not an object.");
        return false;
    }
    auto map = modules.get<nlohmann::json::object_t>();
    std::vector<size_t> moduleIndices{};
    moduleIndices.reserve(modules.size());

    for (const auto& [module, moduleIndex] : map) {
        if (!moduleExists(module)) {
            pushPopUpAlert(std::format(
                "Deserialization error: Module {} is not loaded.",
                module));
            return false;
        }
        if (!moduleIndex.is_number_integer() && !moduleIndex.is_number_unsigned()) {
            pushPopUpAlert(std::format(
                "Deserialization error: Index value of module '{}' is not unsigned integer.",
                module));
            return false;
        }
        moduleIndices.emplace_back(moduleIndex.get<size_t>());
    }

    std::ranges::sort(moduleIndices);
    auto duplicateFindResult = std::ranges::adjacent_find(moduleIndices);
    if (duplicateFindResult != moduleIndices.end()) {
        pushPopUpAlert(std::format(
            "Deserialization error: Index {} appears twice in module orders.",
            *duplicateFindResult));
        return false;
    }
    auto incrementFindResult =
        std::ranges::adjacent_find(moduleIndices,
        [](auto lower, auto higher) {
            return higher != lower + 1;
        }
    );
    if (incrementFindResult != moduleIndices.end()) {
        pushPopUpAlert(std::format(
            "Deserialization error: Module indices should be direct increments."
            " Instead got {} and then {}.", *incrementFindResult,
            *(incrementFindResult + 1)));
        return false;
    }
    return true;
}

bool assertValidModuleSerializations(const nlohmann::json& serializations) {
	return std::ranges::all_of(moduleSerializers, [&serializations](const auto& serializingModule) {
		if (!serializations.contains(serializingModule)) {
			pushPopUpAlert(std::format("Deserialization error: Missing serialization data for"
				"module {}.", serializingModule));
			return false;
		}
		return true;
	}, &std::pair<const std::string, std::function<std::string()>>::first);
}

bool assertValidParticle(const nlohmann::json& particle) {
    for (const auto& attribute : getAttributes()) {
        if (!particle.contains(attribute.name)) {
            pushPopUpAlert(std::format(
                "Deserialization error: Missing attribute '{}' on particle.",
                attribute.name));
            return false;
        }
        const auto& jsonValue = particle[attribute.name];
        if (!jsonValue.is_number()) {
            pushPopUpAlert(std::format(
                "Deserialization error: Attribute '{}' on particle is not numerical value.",
                attribute.name));
            return false;
        }
        auto value = jsonValue.get<float>();
        if (!(value >= attribute.minValue && value <= attribute.maxValue)) {
            return false;
            pushPopUpAlert(std::format(
                "Deserialization error: Attribute '{}' on particle value not in range."
                "Minimum: {}, Maximum: {}, Value: {}.",
                attribute.name, attribute.minValue, attribute.maxValue, value
            ));
        }
    }
    if (!particle.contains("Mass")) {
        pushPopUpAlert("Deserialization error: Particle has no mass.");
        return false;
    }
    const auto& jsonValue = particle["Mass"];
    if (!jsonValue.is_number()) {
        pushPopUpAlert("Deserialization error: Mass of particle is not numerical value.");
        return false;
    }
    auto mass = jsonValue.get<float>();
    if (mass < Particle::minMass || mass > Particle::maxMass) {
        pushPopUpAlert(std::format(
            "Deserialization error: Particle mass is out of allowed range."
            "Minimum: {}, Maximum: {}, Value: {}.",
            Particle::minMass, Particle::maxMass, mass
            ));
        return false;
    }
    return true;
}

bool assertValidConstants(const nlohmann::json& constants) {
    for (const auto& [name, constant] : getAllConstants()) {
        if (!constants.contains(name)) {
            pushPopUpAlert(std::format(
                "Deserialization error: Missing constant '{}'.",
                name
                ));
            return false;
        }
        const auto& jsonValue = constants[name];
        if (!jsonValue.is_number()) {
            pushPopUpAlert(std::format(
                "Deserialization error: Value of constant '{}' is not numerical.",
                name
                ));
            return false;
        }
        auto value = jsonValue.get<float>();
        if (value < constant.minValue || value > constant.maxValue) {
            pushPopUpAlert(std::format(
                "Deserialization error: Constant '{}' is not within allowed range."
                "Minimum: {}, Maximum: {}, Value: {}.",
                name, constant.minValue, constant.maxValue, value
                ));
            return false;
        }
    }
    return true;
}

bool assertValidSave(const nlohmann::json& json) {
    if (!json.contains("Particles")) {
        pushPopUpAlert("Deserialization error: No particles in save file.");
        return false;
    }
    const auto& particles = json["Particles"];
    if (!particles.is_array()) {
        pushPopUpAlert("Deserialization error: Particles in save file are not an array.");
        return false;
    }
    const auto& particlesArray = particles.get<nlohmann::json::array_t>();
    for (const auto& particle : particlesArray) {
        if (!assertValidParticle(particle)) {
            return false;
        }
    }
    if (!json.contains("Constants")) {
        pushPopUpAlert("Deserialization error: No constants in save file.");
        return false;
    }
    const auto& constants = json["Constants"];
    if (!constants.is_object()) {
        pushPopUpAlert("Deserialization error: Constants in save file are not an object.");
        return false;
    }
    if (!assertValidConstants(constants)) {
        return false;
    }
    if (!json.contains("Modules")) {
        pushPopUpAlert("Deserialization error: No modules information in save file.");
        return false;
    }
    const auto& modules = json["Modules"];
    if (!assertValidModules(modules)) {
        return false;
    }
    if (!json.contains("Module-Specific Data")) {
	    pushPopUpAlert("Deserialization error: No Module Serialization data in save file.");
	    return false;
    }
	const auto& moduleData = json["Module-Specific Data"];
	if (!assertValidModuleSerializations(moduleData)) {
		return false;
	}
    return true;
}

} // namespace

void serializeState(const std::string& path) {
    nlohmann::json json;
    serializeModules(json);
    serializeConstants(json);
    json["Particles"] = nlohmann::json::array();
    json["Particles"].get_ref<nlohmann::json::array_t&>().reserve(Particles::get().size());
    for (size_t i = 0; i < Particles::get().size(); i++) {
        serializeParticle(json, i);
    }
    auto serialized = json.dump();
    std::ofstream file(path);
    if (!file) {
        pushPopUpAlert("Serialization error: Could not access selected file.");
        return;
    }
    file << std::setw(4) << serialized << "\n";
    file.flush();
    if (file.fail()) {
        pushPopUpAlert("Serialization error: Failed to write to file.");
        return;
    }
}

void deserializeState(const std::string& path) {
    nlohmann::json json;
    {
        std::ifstream file(path);
        if (!file) {
            pushPopUpAlert("Deserialization error: Could not open file.");
            return;
        }
        try {
            file >> json;
        } catch (const nlohmann::json::parse_error& exception) {
            pushPopUpAlert(std::format("Deserialization error: Could not parse file."
                " Error {} at byte {}. Message: '{}'.",
                exception.id, exception.byte, exception.what()));
            return;
        }
        if (file.fail()) {
            pushPopUpAlert("Deserialization error: Failed to read file.");
            return;
        }
    }
    if (!assertValidSave(json)) {return;}
    deserializeModules(json["Modules"]);
	deserializeModuleData(json["Module-Specific Data"]);
    deserializeConstants(json);
    size_t particleCount = json["Particles"].size();
    Particles::resize(particleCount);
    for (size_t i = 0; i < particleCount; i++) {
        deserializeParticle(i, json["Particles"]);
    }
}

void addModuleSerializer(const std::string& module, std::function<std::string()> serializer) {
	moduleSerializers[module] = serializer;
}

void addModuleDeserializer(const std::string& module, std::function<void(std::string)> deserializer) {
	moduleDeserializers[module] = deserializer;
}

void removeModuleSerializersAndDeserializers(const std::string& module) noexcept {
	std::erase_if(moduleSerializers, [&module](const auto& pair) {
		return pair.first == module;
	});
	std::erase_if(moduleDeserializers, [&module](const auto& pair) {
		return pair.first == module;
	});
}
