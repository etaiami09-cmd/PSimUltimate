#ifndef PSIM_PARTICLE_IMPL_HPP
#define PSIM_PARTICLE_IMPL_HPP

#include "Particle.hpp"

constexpr Particle::Particle() noexcept = default;
constexpr Particle::Particle(const Position position, const Velocity velocity, float radius) noexcept
: position(position), velocity(velocity), radius(radius) {}
constexpr Particle::Particle(const Particle& other) noexcept
: position(other.position), velocity(other.velocity), radius(other.radius) {}

inline const Position& Particle::getPosition() const noexcept {
    return position;
}

inline const Velocity& Particle::getVelocity() const noexcept {
    return velocity;
}

inline float Particle::getRadius() const noexcept {
    return radius;
}

inline void Particle::setPosition(Position newPosition) noexcept {
    this->position = newPosition;
}

inline void Particle::setVelocity(Velocity newVelocity) noexcept {
    this->velocity = newVelocity;
}

inline void Particle::setRadius(float newRadius) noexcept {
    this->radius = newRadius;
}

inline void Particle::tick(float dt) {
    position += velocity * dt;   
}

#endif