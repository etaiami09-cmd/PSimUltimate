#include "simulation.hpp"

namespace {
bool isActive = true;
} // namespace

void Simulation::start() noexcept {
    isActive = true;
}

[[nodiscard]] bool Simulation::on() noexcept {
    return isActive;
}

void Simulation::pause() noexcept {
    isActive = false;
}