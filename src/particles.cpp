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
constexpr float defaultK = 5e3;
float kConstant = defaultK;
} // namespace

std::span<const Particle> Particles::get() noexcept {
    return std::span{particles};
}

void Particles::add(Position pos, Velocity vel, float radius, float mass) {
    particles.emplace_back(pos, vel, radius, mass);
    charges.push_back(0);
    forces.emplace_back();
}

void Particles::reset() noexcept {
    particles.clear();
    charges.clear();
    forces.clear();
}

namespace {
void tickGravity() {
    for (size_t i = 0; i < particles.size(); i++) {
        forces[i] += Force{0, gravity * particles[i].getMass()};
    }
}
void tickElectric() {
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = 0; j < particles.size(); j++) {
            if (i == j) {
                continue;
            }
            auto force = coulombForce(
                ElectricParticle{particles[i], charges[i]},
                ElectricParticle{particles[j], charges[j]}
            );
            forces[i] += force;
            forces[j] -= force;
        }
    }
}
void tickForces() {
    for (size_t i = 0; i < forces.size(); i++) {
        particles[i].setVelocity(particles[i].getVelocity()
                + Velocity{forces[i].x / particles[i].getMass(),
                        forces[i].y / particles[i].getMass()});
        forces[i] = {0, 0};
    }
}
} // namespace

void Particles::tick(float dt) noexcept {
    if (Gravity::on()) {
        tickGravity();
    }
    if (Electric::on()) {
        tickElectric();
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