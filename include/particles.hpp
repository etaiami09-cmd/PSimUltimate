#ifndef PSIM_PARTICLES_HPP
#define PSIM_PARTICLES_HPP

#include <span>
#include <cstddef>
#include "Particle.hpp"
#include <optional>

namespace Particles {
    std::span<const Particle> get() noexcept;
    void add(Position pos, Velocity vel);
    void reset() noexcept;
    void tickGravity() noexcept;
    void tickPositions() noexcept;
} // namespace Particles

namespace Electric {
    using Charge = float;
    void init() noexcept;
    void set(size_t index, Charge charge) noexcept;
    void finish() noexcept;
    std::optional<Charge> get(size_t index) noexcept;
    bool on() noexcept;
} // namespace Electric

#endif