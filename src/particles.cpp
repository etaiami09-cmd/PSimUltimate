#include <cstdio>
#include <vector>
#include <span>

#include "physics_callbacks.hpp"
#include "Particle.hpp"

#include "particles.hpp"

namespace {
std::vector<Particle> particles;
std::vector<Force> forces;
} // namespace

std::span<const Particle> Particles::get() noexcept {
    return std::span{particles};
}

void Particles::add(Position pos, Velocity vel, float radius, float mass) {
    particles.emplace_back(pos, vel, radius, mass);
    forces.emplace_back();
}

void Particles::set(size_t index, Position pos, Velocity vel, float radius, float mass) noexcept {
    if (index < particles.size()) {
        particles[index] = Particle{pos, vel, radius, mass};
    }
}

void Particles::resize(size_t size) noexcept {
    particles.resize(size);
}

void Particles::reset() noexcept {
    particles.clear();
    forces.clear();
}

namespace {
void tickForces() {
    for (size_t i = 0; i < forces.size(); i++) {
        particles[i].setVelocity(particles[i].getVelocity()
                + Velocity{forces[i].x / particles[i].getMass(),
                        forces[i].y / particles[i].getMass()});
        forces[i] = {0, 0};
    }
}
void accumulateForces() {
    for (const auto& forceHandler : getForceHandlers()) {
        if (forceHandler.module.active) {
            forceHandler(particles, forces);
        }
    }
}
void accumulateVelocity() {
    for (const auto& velocityHandler : getVelocityHandlers()) {
        if (velocityHandler.module.active) {
            velocityHandler(particles);
        }
    }
}
void callPositionHandlers() {
    for (const auto& positionHandler : getPositionHandlers()) {
        if (positionHandler.module.active) {
            positionHandler(particles);
        }
    }
}
} // namespace

void Particles::tick(float dt) noexcept {
    accumulateForces();
    tickForces();
    accumulateVelocity();
    for (auto& part : particles) {
        part.tick(dt);
    }
    callPositionHandlers();
}