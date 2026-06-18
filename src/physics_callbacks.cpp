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

std::span<const ForceHandler> getForceHandlers() noexcept {
    return forceHandlers;
}

std::span<const VelocityHandler> getVelocityHandlers() noexcept {
    return velocityHandlers;
}
std::span<const PositionHandler> getPositionHandlers() noexcept {
    return positionHandlers;
}