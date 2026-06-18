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

namespace {

std::unordered_map<std::string, float> buildConstantsMap() {
    std::unordered_map<std::string, float> map;
    for (const auto& constant : getAllConstants()) {
        map[constant.first] = constant.second.value;
    }
    return map;
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
    addNewParticleAttributes(attributeValues);
    Particles::set(index, position, velocity, radius, mass);
}
} // namespace

void serializeState(const std::string& path) {
    nlohmann::json json;
    serializeConstants(json);
    json["Particles"] = nlohmann::json::array();
    json["Particles"].get_ref<nlohmann::json::array_t&>().reserve(Particles::get().size());
    for (size_t i = 0; i < Particles::get().size(); i++) {
        serializeParticle(json, i);
    }
    auto serialized = json.dump();
    std::ofstream file(path);
    file << std::setw(4) << serialized << "\n";
}

void deserializeState(const std::string& path) {
    nlohmann::json json;
    {
        std::ifstream file(path);
        file >> json;
    }
    deserializeConstants(json);
    size_t particleCount = json["Particles"].size();
    Particles::resize(particleCount);
    clearAttributeValues();
    for (size_t i = 0; i < particleCount; i++) {
        deserializeParticle(i, json["Particles"]);
    }
}