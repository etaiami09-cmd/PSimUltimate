#ifndef PSIM_PARTICLE_HPP
#define PSIM_PARTICLE_HPP

#include "Vec2.hpp"
#include "inlines/Vec2-impl.hpp"

namespace {
template<typename T>
using FloatVecOps = PSimImpl::VectorOperators<float, T>;
} // namespace

struct Position : Vec2<float>, FloatVecOps<Position> {
    using Vec2::Vec2;
    using FloatVecOps<Position>::operator+;
    using FloatVecOps<Position>::operator-;
};

struct Velocity : Vec2<float>, FloatVecOps<Velocity> {
    using Vec2::Vec2;
    using FloatVecOps<Velocity>::operator+;
    using FloatVecOps<Velocity>::operator-;
};

class Particle {
private:
    Position position;
    Velocity velocity;
    float radius;
public:
    constexpr Particle() noexcept;
    constexpr Particle(Position position, Velocity velocity, float radius) noexcept;
    constexpr Particle(const Particle& other) noexcept;
    const Position& getPosition() const noexcept;
    const Velocity& getVelocity() const noexcept;
    float getRadius() const noexcept;
    void setPosition(Position newPosition) noexcept;
    void setVelocity(Velocity newVelocity) noexcept;
    void setRadius(float newRadius) noexcept;
    void tick(float dt);
};

#include "Particle-impl.hpp"

#endif