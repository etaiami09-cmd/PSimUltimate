#include <vector>
#include <span>
#include <cstddef>

#include "physics_callbacks.hpp"
#include "Particle.hpp"
#include "module.hpp"
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
    forces.resize(size);
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
        const Module& module = getModules()[forceHandler.moduleIndex];
        if (module.active) {
            forceHandler(particles, forces);
        }
    }
}
void accumulateVelocity() {
    for (const auto& velocityHandler : getVelocityHandlers()) {
        const Module& module = getModules()[velocityHandler.moduleIndex];
        if (module.active) {
            velocityHandler(particles);
        }
    }
}
void callPositionHandlers() {
    for (const auto& positionHandler : getPositionHandlers()) {
        const Module& module = getModules()[positionHandler.moduleIndex];
        if (module.active) {
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