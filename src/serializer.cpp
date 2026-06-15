#include <string>
#include <cstddef>
#include <fstream>
#include <iomanip>

#include "serializer.hpp"

#include "Particle.hpp"
#include "json.hpp"
#include "particles.hpp"

namespace {
void serializeConstants(nlohmann::json& json) {
    json["Constants"] = {
        {"Gravity", Gravity::get()},
        {"Coulomb", Electric::getK()}
    };
}
void serializeParticle(nlohmann::json& json, size_t index) {
    auto mass = Particles::get()[index].getMass();
    auto position = Particles::get()[index].getPosition();
    auto velocity = Particles::get()[index].getVelocity();
    auto radius = Particles::get()[index].getRadius();
    auto charge = Electric::get(index);
    auto rawCharge = charge.has_value() ? charge.value() : 0;
    nlohmann::json particle;
    particle["Mass"] = mass;
    particle["Position"] = {position.x, position.y};
    particle["Velocity"] = {velocity.x, velocity.y};
    particle["Charge"] = rawCharge;
    particle["Radius"] = radius;
    json["Particles"].push_back(particle);
}

void deserializeConstants(nlohmann::json& json) {
    nlohmann::json constants = json["Constants"];
    if (!constants["Gravity"].empty()) {
        Gravity::set(constants["Gravity"].get<float>());
    }
    if (!constants["Coulomb"].empty()) {
        Electric::setK(constants["Coulomb"].get<float>());
    }
}
void deserializeParticle(size_t index, nlohmann::json& particles) {
    auto mass = particles[index]["Mass"].get<float>();
    auto position = Position{
        particles[index]["Position"][0].get<float>(),
        particles[index]["Position"][1].get<float>()};
    auto velocity = Velocity{
        particles[index]["Velocity"][0].get<float>(),
        particles[index]["Velocity"][1].get<float>()
    };
    auto charge = particles[index]["Charge"].get<float>();
    auto radius = particles[index]["Radius"].get<float>();
    Particles::set(index, position, velocity, radius, mass);
    Electric::set(index, charge);
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
    for (size_t i = 0; i < particleCount; i++) {
        deserializeParticle(i, json["Particles"]);
    }
}