#ifndef PSIM_PARTICLE_IMPL_HPP
#define PSIM_PARTICLE_IMPL_HPP

#include "Particle.hpp"

constexpr Particle::Particle() noexcept = default;
constexpr Particle::Particle(const Position position, const Velocity velocity, float radius, float mass) noexcept
: position(position), velocity(velocity), radius(radius), mass(mass) {}

[[nodiscard]] inline const Position& Particle::getPosition() const noexcept {
    return position;
}

[[nodiscard]] inline const Velocity& Particle::getVelocity() const noexcept {
    return velocity;
}

[[nodiscard]] inline float Particle::getRadius() const noexcept {
    return radius;
}

[[nodiscard]] inline float Particle::getMass() const noexcept {
    return mass;
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

inline void Particle::setMass(float newMass) noexcept {
    this->mass = newMass;
}

inline void Particle::tick(float dt) {
    position += velocity * dt;   
}

#endif