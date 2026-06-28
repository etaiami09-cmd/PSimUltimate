#ifndef PSIM_PARTICLES_HPP
#define PSIM_PARTICLES_HPP

#include <span>
#include <cstddef>
#include <vector>
#include "Particle.hpp"

constexpr float defaultDt = 0.016f;

namespace Particles {
    std::span<const Particle> get() noexcept;
    std::span<const Force> getForces() noexcept;
    void add(Position pos, Velocity vel, float radius, float mass, const std::vector<float>& attributes);
    void set(size_t index, Position pos, Velocity vel, float radius, float mass) noexcept;
    void resize(size_t size) noexcept;
    void reset() noexcept;
    void tick(float dt=defaultDt) noexcept;
} // namespace Particles

#endif