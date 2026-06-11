#ifndef PSIM_ELECTROSTATIC_HPP
#define PSIM_ELECTROSTATIC_HPP

#include "Particle.hpp"
#include "particles.hpp"

struct ElectricParticle {
    const Particle& particle;
    const Electric::Charge charge;
};

Force coulombForce(const ElectricParticle& a, const ElectricParticle& b) noexcept;

#endif