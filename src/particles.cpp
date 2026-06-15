#include <cstdio>
#include <vector>
#include <span>
#include <optional>

#include "Particle.hpp"
#include "electrostatic.hpp"

#include "particles.hpp"

namespace {
std::vector<Particle> particles;
std::vector<Force> forces;
bool electricActive;
std::vector<Electric::Charge> charges;
constexpr float defaultGravity = 5;
float gravity = defaultGravity;
bool gravityActive = false;
constexpr float defaultK = 5e05;
float kConstant = defaultK;
} // namespace

std::span<const Particle> Particles::get() noexcept {
    return std::span{particles};
}

void Particles::add(Position pos, Velocity vel, float radius) {
    particles.emplace_back(pos, vel, radius);
    charges.push_back(0);
    forces.emplace_back();
}

void Particles::reset() noexcept {
    particles.clear();
    charges.clear();
    forces.clear();
}

namespace {
void tickGravity(float dt) {
    for (auto& particle : particles) {
        particle.setVelocity(particle.getVelocity() + Velocity{0, Gravity::get() * dt});
    }
}
void tickElectric(float dt) {
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = 0; j < particles.size(); j++) {
            if (i == j) {
                continue;
            }
            auto force = coulombForce(
                ElectricParticle{particles[i], charges[i]},
                ElectricParticle{particles[j], charges[j]}
            ) * (dt / 2);
            forces[i] += force;
            forces[j] -= force;
        }
    }
}
void tickForces() {
    // placeholder - just copies forces to velocity. mass does not exist yet
    for (size_t i = 0; i < forces.size(); i++) {
        particles[i].setVelocity(particles[i].getVelocity() + Velocity{forces[i].x, forces[i].y});
        forces[i] = {0, 0};
    }
}
} // namespace

void Particles::tick(float dt) noexcept {
    if (Gravity::on()) {
        tickGravity(dt);
    }
    if (Electric::on()) {
        tickElectric(dt);
    }
    tickForces();
    for (auto& part : particles) {
        part.tick(dt);
    }
}

void Gravity::init() noexcept {
    gravityActive = true;
}

[[nodiscard]] bool Gravity::on() noexcept {
    return gravityActive;
}

void Gravity::set(float newGravity) noexcept {
    gravity = newGravity;
}

[[nodiscard]] float Gravity::getDefault() noexcept {
    return defaultGravity;
}

[[nodiscard]] float Gravity::get() noexcept {
    return gravity;
}

void Gravity::finish() noexcept {
    gravityActive = false;
}

void Gravity::toggle() noexcept {
    gravityActive = !gravityActive;
}

void Electric::init() noexcept {
    if (!electricActive) {
        electricActive = true;
    }
}

void Electric::set(size_t index, Charge charge) noexcept {
    if (index < charges.size()) {
        charges[index] = charge;
    }
}

void Electric::finish() noexcept {
    electricActive = false;
}

void Electric::toggle() noexcept {
    if (Electric::on()) {
        Electric::finish();
    }
    else {
        Electric::init();
    }
}

void Electric::setK(float newK) noexcept {
    kConstant = newK;
}

float Electric::getK() noexcept {
    return kConstant;
}

std::optional<Electric::Charge> Electric::get(size_t index) noexcept {
    if (index >= charges.size()) {
        return {};
    }
    return charges[index];
}

[[nodiscard]] bool Electric::on() noexcept {
    return electricActive;
}