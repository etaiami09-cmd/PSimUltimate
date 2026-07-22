#ifndef PSIM_GRAPHICS_CALLBACKS_HPP
#define PSIM_GRAPHICS_CALLBACKS_HPP

#include <span>

#include "module.hpp"
#include "Particle.hpp"

using GraphicsHandler = ModuleCallback<std::span<const Particle>>;

void addGraphicsHandler(const GraphicsHandler& graphicsHandler) noexcept;
std::span<const GraphicsHandler> getGraphicsHandlers() noexcept;
void cleanModuleGraphicsCallbacks(size_t moduleIndex) noexcept;

#endif