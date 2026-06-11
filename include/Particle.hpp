#ifndef PSIM_PARTICLE_HPP
#define PSIM_PARTICLE_HPP

#include "Vec2.hpp"
#include "inlines/Vec2-impl.hpp"

namespace PSimImpl {
template<typename T>
using FloatVecOps = PSimImpl::VectorOperators<float, T>;
} // namespace PSimImpl

struct Position : Vec2<float>, PSimImpl::FloatVecOps<Position> {
    using Vec2::Vec2;
    using PSimImpl::FloatVecOps<Position>::VectorOperators;
    using PSimImpl::FloatVecOps<Position>::operator+;
    using PSimImpl::FloatVecOps<Position>::operator-;
};

struct Velocity : Vec2<float>, PSimImpl::FloatVecOps<Velocity> {
    using Vec2::Vec2;
    using PSimImpl::FloatVecOps<Velocity>::VectorOperators;
    using PSimImpl::FloatVecOps<Velocity>::operator+;
    using PSimImpl::FloatVecOps<Velocity>::operator-;
};

struct Force : Vec2<float>, PSimImpl::FloatVecOps<Force> {
    using Vec2::Vec2;
    using PSimImpl::FloatVecOps<Force>::VectorOperators;
    using PSimImpl::FloatVecOps<Force>::operator+;
    using PSimImpl::FloatVecOps<Force>::operator-;
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
    [[nodiscard]] const Position& getPosition() const noexcept;
    [[nodiscard]] const Velocity& getVelocity() const noexcept;
    [[nodiscard]] float getRadius() const noexcept;
    void setPosition(Position newPosition) noexcept;
    void setVelocity(Velocity newVelocity) noexcept;
    void setRadius(float newRadius) noexcept;
    void tick(float dt);
};

#include "Particle-impl.hpp" // IWYU pragma: export

#endif