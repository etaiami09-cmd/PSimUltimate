#ifndef PSIM_PARTICLES_HPP
#define PSIM_PARTICLES_HPP

#include <span>
#include <cstddef>
#include "Particle.hpp"
#include <optional>

constexpr float defaultDt = 0.016f;

namespace Particles {
    std::span<const Particle> get() noexcept;
    void add(Position pos, Velocity vel, float radius, float mass);
    void reset() noexcept;
    void tick(float dt=defaultDt) noexcept;
} // namespace Particles

namespace Gravity {
    void init() noexcept;
    [[nodiscard]] bool on() noexcept;
    void set(float newGravity) noexcept;
    [[nodiscard]] float get() noexcept;
    [[nodiscard]] float getDefault() noexcept;
    void toggle() noexcept;
    void finish() noexcept;
} // namespace Gravity

namespace Electric {
    using Charge = float;
    void init() noexcept;
    void set(size_t index, Charge charge) noexcept;
    void finish() noexcept;
    void toggle() noexcept;
    void setK(float newK) noexcept;
    [[nodiscard]] float getK() noexcept;
    std::optional<Charge> get(size_t index) noexcept;
    [[nodiscard]] bool on() noexcept;
} // namespace Electric

#endif