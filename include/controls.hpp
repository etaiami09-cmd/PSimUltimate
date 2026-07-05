#ifndef PSIM_CONTROLS_HPP
#define PSIM_CONTROLS_HPP

#include <vector>
#include <string>
#include <functional>

#include "raylib.h"

struct Keybind {
    std::vector<KeyboardKey> keys;
    std::vector<MouseButton> buttons;
    std::string module;
    std::string name;
    std::function<void()> callback;

    Keybind(const std::string& module, const std::string& name,
        std::span<const KeyboardKey> keys, std::span<const MouseButton> buttons,
            const std::function<void()>& callback) noexcept;
};

void handleControls();

void registerKeybinds() noexcept;

void addKeybind(const std::string& module, const std::string& name,
    std::span<const KeyboardKey> keys, std::span<const MouseButton> buttons,
    const std::function<void()>& callback) noexcept;

#endif