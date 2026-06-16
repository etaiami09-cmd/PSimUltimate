#ifndef PSIM_CONTROLS_HPP
#define PSIM_CONTROLS_HPP

#include <vector>
#include <functional>

#include "raylib.h"

struct Keybind {
    std::vector<KeyboardKey> keys;
    std::function<void()> callback;

    Keybind(const std::vector<KeyboardKey>& keys, const std::function<void()>& callback) noexcept;
};

void handleControls();

void registerKeybinds() noexcept;

#endif