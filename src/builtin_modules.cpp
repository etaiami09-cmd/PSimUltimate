#include <span>
#include <string>
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

namespace {
constexpr float maxForce = 10;
constexpr float distancePadding = 0.000001;

struct ElectricParticle {
    const Particle &particle;
    const float charge;
};

Force coulombForce(const ElectricParticle &a, const ElectricParticle &b, float coulombs) noexcept {
    auto deltas = a.particle.getPosition() - b.particle.getPosition();
    auto distanceSquared = deltas.magnitude_squared();
    auto force = (coulombs * a.charge * b.charge) / (distanceSquared + distancePadding);
    force = std::clamp(force, -maxForce * maxForce, maxForce * maxForce);
    auto distance = std::sqrt(distanceSquared + distancePadding);
    auto factor = force / distance;
    return Force{deltas * factor};
}

void tickElectric(std::span<Particle> particles, std::span<Force> forces) {
    const auto charges = getAttributeByName("Charge").values;
    auto coulombsOptional = getConstantValue("Coulomb's");
    if (!coulombsOptional.has_value()) { return; }
    float coulombsValue = coulombsOptional.value();
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
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

constexpr float defaultGravity = 5;
float gravity = defaultGravity;
constexpr float defaultK = 9e03;
float coulombs = defaultK;

void tickGravity(std::span<Particle> particles, std::span<Force> forces) {
    for (size_t i = 0; i < particles.size(); i++) {
        forces[i] += Force{0, gravity * particles[i].getMass()};
    }
}
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
