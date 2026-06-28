#ifndef PSIM_PARTICLE_HPP
#define PSIM_PARTICLE_HPP

#include <limits>

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
    float mass;
public:
    constexpr Particle() noexcept;
    constexpr Particle(Position position, Velocity velocity, float radius, float mass) noexcept;
    constexpr Particle(const Particle& other) noexcept = default;
    [[nodiscard]] const Position& getPosition() const noexcept;
    [[nodiscard]] const Velocity& getVelocity() const noexcept;
    [[nodiscard]] float getRadius() const noexcept;
    [[nodiscard]] float getMass() const noexcept;
    void setPosition(Position newPosition) noexcept;
    void setVelocity(Velocity newVelocity) noexcept;
    void setRadius(float newRadius) noexcept;
    void setMass(float newMass) noexcept;
    void tick(float dt);
    constexpr static float minMass = 0;
    constexpr static float minRadius = 0;
    constexpr static float maxMass = std::numeric_limits<float>::max();
    constexpr static float maxRadius = std::numeric_limits<float>::max();

};

#include "Particle-impl.hpp" // IWYU pragma: export

#endif