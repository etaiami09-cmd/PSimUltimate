#ifndef PSIM_SIMULATION_HPP
#define PSIM_SIMULATION_HPP

namespace Simulation {
void start() noexcept;
[[nodiscard]] bool on() noexcept;
void pause() noexcept;
} // namespace Simulation

#endif