#include <vector>
#include <span>

#include "attributes.hpp"
#include "physics_callbacks.hpp"
#include "Particle.hpp"
#include "module.hpp"
#include "particles.hpp"

#include "configs.hpp"

namespace {
std::vector<Particle> particles;
std::vector<Force> forces;
int ticksPerFrame;
constexpr int defaultTicksPerFrame = 1;
} // namespace

std::span<const Particle> Particles::get() noexcept {
    return std::span{particles};
}

std::span<const Force> Particles::getForces() noexcept {
    return std::span{forces};
}

void Particles::add(Position pos, Velocity vel, float radius, float mass, const std::vector<float>& attributes) {
    particles.emplace_back(pos, vel, radius, mass);
    forces.emplace_back();
    addNewParticleAttributes(attributes);
}

void Particles::set(size_t index, Position pos, Velocity vel, float radius, float mass) noexcept {
    if (index < particles.size()) {
        particles[index] = Particle{pos, vel, radius, mass};
    }
}

void Particles::resize(size_t size) noexcept {
    particles.resize(size);
    forces.resize(size);
    resizeAttributeValues(size);
}

void Particles::reset() noexcept {
    particles.clear();
    forces.clear();
    clearAttributeValues();
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
    for (const auto moduleIndex : getModuleIndexOrders()) {
        const auto& module = getModules()[moduleIndex];
        if (module.active) {
            for (const auto& forceHandler : getForceHandlers()) {
                if (forceHandler.moduleIndex == moduleIndex) {
                    forceHandler(particles, forces);
                }
            }
        }
    }
}
void accumulateVelocity() {
    for (const auto moduleIndex : getModuleIndexOrders()) {
        const auto& module = getModules()[moduleIndex];
        if (module.active) {
            for (const auto& velocityHandler : getVelocityHandlers()) {
                if (velocityHandler.moduleIndex == moduleIndex) {
                    velocityHandler(particles);
                }
            }
        }
    }
}
void callPositionHandlers() {
    for (const auto moduleIndex : getModuleIndexOrders()) {
        const auto& module = getModules()[moduleIndex];
        if (module.active) {
            for (const auto& positionHandler : getPositionHandlers()) {
                if (positionHandler.moduleIndex == moduleIndex) {
                    positionHandler(particles);
                }
            }
        }
    }
}
} // namespace

void Particles::tick() noexcept {
    float tickCount = getTicksPerFrame();
    float dt = (1.0f / (getFPS() * tickCount));
    for (int i = 0; i < tickCount; i++) {
        accumulateForces();
        tickForces();
        accumulateVelocity();
        for (auto& part : particles) {
            part.tick(dt);
        }
        callPositionHandlers();
    }
}

int getTicksPerFrame() {
    return ticksPerFrame;
}

void setTicksPerFrame(int ticks) {
    ticksPerFrame = ticks;
    setConfigValue<int>("TicksPerFrame", ticksPerFrame);
}

void loadTicksPerFrameConfig() {
    auto optionalValue = getConfigValue<int>("TicksPerFrame");
    if (!optionalValue.has_value()) {
        setConfigValue<int>("TicksPerFrame", defaultTicksPerFrame);
        setTicksPerFrame(defaultTicksPerFrame);
        return;
    }
    setTicksPerFrame(std::get<0>(optionalValue.value()));
}