#include <vector>
#include <span>

#include "physics_callbacks.hpp"

namespace {
std::vector<ForceHandler> forceHandlers;
std::vector<VelocityHandler> velocityHandlers;
std::vector<PositionHandler> positionHandlers;
} // namespace

void addForceHandler(const ForceHandler& forceHandler) noexcept {
    forceHandlers.push_back(forceHandler);
}

void addVelocityHandler(const VelocityHandler& velocityHandler) noexcept {
    velocityHandlers.push_back(velocityHandler);
}

void addPositionHandler(const PositionHandler& positionHandler) noexcept {
    positionHandlers.push_back(positionHandler);
}

void cleanModulePhysicsCallbacks(size_t moduleIndex) noexcept {
	std::erase_if(forceHandlers, [&moduleIndex](const auto& forceHandler) {
		return forceHandler.moduleIndex == moduleIndex;
	});
	std::erase_if(velocityHandlers, [&moduleIndex](const auto& velocityHandler) {
		return velocityHandler.moduleIndex == moduleIndex;
	});
	std::erase_if(positionHandlers, [&moduleIndex](const auto& positionHandler) {
		return positionHandler.moduleIndex == moduleIndex;
	});
	for (auto& forceHandler : forceHandlers) {
		if (forceHandler.moduleIndex > moduleIndex) {
			forceHandler.moduleIndex--;
		}
	}
	for (auto& velocityHandler : velocityHandlers) {
		if (velocityHandler.moduleIndex > moduleIndex) {
			velocityHandler.moduleIndex--;
		}
	}
	for (auto& positionHandler : positionHandlers) {
		if (positionHandler.moduleIndex > moduleIndex) {
			positionHandler.moduleIndex--;
		}
	}
}

std::span<const ForceHandler> getForceHandlers() noexcept {
    return forceHandlers;
}

std::span<const VelocityHandler> getVelocityHandlers() noexcept {
    return velocityHandlers;
}
std::span<const PositionHandler> getPositionHandlers() noexcept {
    return positionHandlers;
}