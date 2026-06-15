#include <algorithm>
#include <cmath>

#include "Particle.hpp"
#include "particles.hpp"

#include "electrostatic.hpp"

namespace {
constexpr float maxForce = 10;
constexpr float distancePadding = 0.000001;
} // namespace

Force coulombForce(const ElectricParticle& a, const ElectricParticle& b) noexcept {
    auto deltas = a.particle.getPosition() - b.particle.getPosition();
    auto distanceSquared = deltas.magnitude_squared();
    auto force = (Electric::getK() * a.charge * b.charge) / (distanceSquared + distancePadding);
    force = std::clamp(force, - maxForce * maxForce, maxForce * maxForce);
    auto distance = std::sqrt(distanceSquared);
    auto factor = force / distance;
    return Force{deltas * factor};
}