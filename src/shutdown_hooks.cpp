#include "shutdown_hooks.hpp"

namespace {
std::vector<std::function<void()>> shutdownHooks;
} // namespace

void addShutdownHook(const std::function<void()>& hook) {
	shutdownHooks.push_back(hook);
}

void runShutdownHooks() {
	for (const auto& hook : shutdownHooks) {
		hook();
	}
}
