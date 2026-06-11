#include <cmath>

#include "Particle.hpp"
#include "particles.hpp"

#include "electrostatic.hpp"

Force coulombForce(const ElectricParticle& a, const ElectricParticle& b) noexcept {
    auto deltas = a.particle.getPosition() - b.particle.getPosition();
    auto distanceSquared = deltas.magnitude_squared();
    auto force = (Electric::getK() * a.charge * b.charge) / distanceSquared;
    auto distance = std::sqrt(distanceSquared);
    auto factor = force / distance;
    return Force{deltas * factor};
}