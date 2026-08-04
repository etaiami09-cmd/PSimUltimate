#ifndef PSIM_SHUTDOWN_HOOKS_HPP
#define PSIM_SHUTDOWN_HOOKS_HPP

#include <functional>

void addShutdownHook(const std::function<void()>& hook);

void runShutdownHooks();

#endif