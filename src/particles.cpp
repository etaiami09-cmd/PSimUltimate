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
constexpr float defaultDt = 0.016;
constexpr float gravity = 5;
} // namespace

std::span<const Particle> Particles::get() noexcept {
        return std::span{particles};
}

void Particles::add(Position pos, Velocity vel) {
    particles.emplace_back(pos, vel);
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

void Particles::tickGravity() noexcept {
    for (auto& part : particles) {
        part.setVelocity(part.getVelocity() + Velocity{0, gravity});
    }
}

void Particles::tickPositions() noexcept {
    for (auto& part : particles) {
        part.tick(defaultDt);
    }
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

bool Electric::on() noexcept {
    return electricActive;
}