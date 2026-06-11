#include <cstdio>
#include <vector>
#include <span>
#include <optional>

#include "Particle.hpp"

#include "particles.hpp"

namespace {
std::vector<Particle> particles;
bool electricActive;
std::vector<Electric::Charge> charges;
constexpr float defaultGravity = 5;
float gravity = defaultGravity;
bool gravityActive = false;
} // namespace

std::span<const Particle> Particles::get() noexcept {
        return std::span{particles};
}

void Particles::add(Position pos, Velocity vel, float radius) {
    particles.emplace_back(pos, vel, radius);
    if (electricActive) {
        charges.push_back(0);
    }
}

void Particles::reset() noexcept {
    particles.clear();
    if (electricActive) {
        charges.clear();
    }
}

namespace {
void tickGravity(float dt) {
    for (auto& particle : particles) {
        particle.setVelocity(particle.getVelocity() + Velocity{0, Gravity::get() * dt});
    }
}
void tickElectric(float dt) {
    for (auto& particle : particles) {
        // particle.setVelocity(particle.getVelocity()); // TODO: IMPLEMENT ELECTROSTATIC FORCES
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

[[nodiscard]] float Gravity::get() noexcept {
    return gravity;
}

void Gravity::finish() noexcept {
    gravityActive = false;
}

void Electric::init() noexcept {
    if (!electricActive) {
        electricActive = true;
        charges = std::vector<Charge>(particles.size());
    }
}

void Electric::set(size_t index, Charge charge) noexcept {
    if (index < charges.size()) {
        charges[index] = charge;
    }
}

void Electric::finish() noexcept {
    electricActive = false;
    charges.clear();
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