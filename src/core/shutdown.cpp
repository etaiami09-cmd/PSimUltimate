#include "confirm_action.hpp"
#include "graphical.hpp"
#include "raylib.h"

#include "shutdown.hpp"

namespace {
bool confirmedShutdown = false;
} // namespace

void shutdownApplication() {
	confirmAction("Exit Program",
		"Are you sure you want to exit the application? This could lead to loss of unsaved data.",
		[]() {
			confirmedShutdown = true;
		});
}

bool shouldShutdown() {
	return confirmedShutdown;
}
