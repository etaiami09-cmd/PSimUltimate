#ifndef PSIM_PARTICLES_HPP
#define PSIM_PARTICLES_HPP

#include <span>
#include <cstddef>
#include <vector>

#include "fps.hpp"
#include "Particle.hpp"

int getTicksPerFrame();
void setTicksPerFrame(int ticks);
void loadTicksPerFrameConfig();

namespace Particles {
    std::span<const Particle> get() noexcept;
    std::span<const Force> getForces() noexcept;
    void add(Position pos, Velocity vel, float radius, float mass, const std::vector<float>& attributes);
    void set(size_t index, Position pos, Velocity vel, float radius, float mass) noexcept;
    void resize(size_t size) noexcept;
    void reset() noexcept;
    void tick() noexcept;
} // namespace Particles

#endif