#ifndef PSIM_PHYSICS_CALLBACK_HPP
#define PSIM_PHYSICS_CALLBACK_HPP

#include <span>

#include "module.hpp"
#include "Particle.hpp"

using ForceHandler = ModuleCallback<std::span<Particle>, std::span<Force>>;

using VelocityHandler = ModuleCallback<std::span<Particle>>;

using PositionHandler = ModuleCallback<std::span<Particle>>;

void addForceHandler(const ForceHandler& forceHandler) noexcept;
void addVelocityHandler(const VelocityHandler& velocityHandler) noexcept;
void addPositionHandler(const PositionHandler& positionHandler) noexcept;
void cleanModulePhysicsCallbacks(size_t moduleIndex) noexcept;

std::span<const ForceHandler> getForceHandlers() noexcept;
std::span<const VelocityHandler> getVelocityHandlers() noexcept;
std::span<const PositionHandler> getPositionHandlers() noexcept;

#endif