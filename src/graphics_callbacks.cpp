#include <span>
#include <vector>

#include "graphics_callbacks.hpp"

namespace {

std::vector<GraphicsHandler> graphicsHandlers;

} // namespace

void addGraphicsHandler(const GraphicsHandler& graphicsHandler) noexcept {
    graphicsHandlers.push_back(graphicsHandler);
}

std::span<const GraphicsHandler> getGraphicsHandlers() noexcept {
    return graphicsHandlers;
}