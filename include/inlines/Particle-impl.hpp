#ifndef PSIM_PARTICLE_IMPL_HPP
#define PSIM_PARTICLE_IMPL_HPP

#include "Particle.hpp"

constexpr Particle::Particle() noexcept = default;
constexpr Particle::Particle(const Position position, const Velocity velocity) noexcept
: position(position), velocity(velocity) {}
constexpr Particle::Particle(const Particle& other) noexcept
: position(other.position), velocity(other.velocity) {}

inline const Position& Particle::getPosition() const noexcept {
    return position;
}

inline const Velocity& Particle::getVelocity() const noexcept {
    return velocity;
}

inline void Particle::setPosition(Position newPosition) noexcept {
    this->position = newPosition;
}

inline void Particle::setVelocity(Velocity newVelocity) noexcept {
    this->velocity = newVelocity;
}

inline void Particle::tick(float dt) {
    position += velocity * dt;   
}

#endif