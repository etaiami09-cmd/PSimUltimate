#include <algorithm>
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

void cleanModuleGraphicsCallbacks(size_t moduleIndex) noexcept {
	std::erase_if(graphicsHandlers, [&moduleIndex](const auto& graphicsHandler) {
		return graphicsHandler.moduleIndex == moduleIndex;
	});
	for (auto& graphicsHandler : graphicsHandlers) {
		if (graphicsHandler.moduleIndex > moduleIndex) {
			graphicsHandler.moduleIndex--;
		}
	}
}
