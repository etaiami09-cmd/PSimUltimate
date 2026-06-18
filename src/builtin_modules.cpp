#include <span>
#include <string>
#include <cstddef>
#include <limits>
#include <algorithm>
#include <cmath>

#include "constants.hpp"
#include "graphical.hpp"
#include "raylib.h"
#include "Particle.hpp"
#include "attributes.hpp"
#include "builtin_modules.hpp"
#include "psim_module_api.hpp"
// HERE IS WHAT YOU NEED TO DO
// 1. IMPLEMENT GLOBAL STATE IN THE PSIM_MODULE_API HEADER
// SO THAT IT **DEFINES** A C ABI INITIALIZE MODULE FUNCTION
// THAT LOADS A C++ FUNCTION WHICH WAS FORWARD-DECLARED IN THE HEADER
// AND IS LATER DEFINED IN THE MODULE AUTHOR'S C++ MAIN FILE
// THE INITIALIZER FUNCTION IS RESPONSIBLE FOR LOADING ALL THE ABI
// FUNCTIONS INTO GLOBAL STATE AND RESTRUCTURING THEM INTO
// THAT CAN BE CALLED BY THE MODULE AUTHOR
// EFFECTIVELY GIVING THE SAME API I WANTED
// 2. ENJOY BEING AWESOME
namespace {

constexpr float maxForce = 10;
constexpr float distancePadding = 0.000001;

struct ElectricParticle {
    const Particle& particle;
    const float charge;
};

Force coulombForce(const ElectricParticle& a, const ElectricParticle& b, float coulombs) noexcept {
    auto deltas = a.particle.getPosition() - b.particle.getPosition();
    auto distanceSquared = deltas.magnitude_squared();
    auto force = (coulombs * a.charge * b.charge) / (distanceSquared + distancePadding);
    force = std::clamp(force, - maxForce * maxForce, maxForce * maxForce);
    auto distance = std::sqrt(distanceSquared);
    auto factor = force / distance;
    return Force{deltas * factor};
}

void tickElectric(std::span<Particle> particles, std::span<Force> forces) {
    const auto charges = getAttributeByName("Charge").values;
    auto coulombsOptional = getConstantValue("Coulomb's");
    if (!coulombsOptional.has_value()) {return;}
    float coulombsValue = coulombsOptional.value();
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = 0; j < particles.size(); j++) {
            if (i == j) {
                continue;
            }
            auto force = coulombForce(
                ElectricParticle{particles[i], charges[i]},
                ElectricParticle{particles[j], charges[j]},
                coulombsValue
            );
            forces[i] += force;
            forces[j] -= force;
        }
    }
}

void drawElectricParticles(std::span<const Particle> particles) {
    auto charges = getAttributeByName("Charge");
    for (size_t i = 0; i < particles.size(); i++) {
        auto color = GRAY;
        if (auto charge = charges.values[i]; charge != 0) {
            color = (charge > 0) ? BLUE : RED;
        }
        DrawCircle(
            static_cast<int>(particles[i].getPosition().x
                                + static_cast<float>(getPSimGUIWidth())),
            static_cast<int>(particles[i].getPosition().y),
            particles[i].getRadius(), color
        );
    }
}

void tickGravity(std::span<Particle> particles, std::span<Force> forces) {
    auto gravityOptional = getConstantValue("Gravity");
    if (!gravityOptional.has_value()) {return;}
    float gravity = gravityOptional.value();
    for (size_t i = 0; i < particles.size(); i++) {
        forces[i] += Force{0, gravity * particles[i].getMass()};
    }
}

constexpr float defaultGravity = 5;
float gravity = defaultGravity;
constexpr float defaultK = 9e03;
float coulombs = defaultK;

} // namespace

void initElectrostaticModule() noexcept {
    std::string module{"Electric"};
    registerModule(module);
    registerConstant(module, "Coulomb's", defaultK,
        0, std::numeric_limits<float>::max(),
        [](float newK) {
        coulombs = newK;
    });
    registerParticleAttribute(module, "Charge", 0,
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max());
    registerForce(module, tickElectric);
    registerRenderer(module, drawElectricParticles);
}

void initGravityModule() noexcept {
    std::string module{"Gravity"};
    registerModule(module);
    registerConstant(module, "Gravity", defaultGravity,
        0, std::numeric_limits<float>::max(),
    [](float newGravity) {
        gravity = newGravity;
    });
    registerForce(module, tickGravity);
}