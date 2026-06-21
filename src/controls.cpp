#include <vector>
#include <algorithm>
#include <functional>

#include "raylib.h"
#include "portable-file-dialogs.h" // IWYU pragma: export
#include "file_save_open.hpp"

#include "controls.hpp"

namespace {
std::vector<Keybind> keybinds;
} // namespace

Keybind::Keybind(const std::vector<KeyboardKey>& keys, const std::function<void()>& callback) noexcept
: keys(keys), callback(callback) {}

void handleControls() {
    for (const auto& keybind : keybinds) {
        if (std::ranges::all_of(keybind.keys.begin(), keybind.keys.end(),
            IsKeyDown))
        {
            keybind.callback();
        }
    }
}

void registerKeybinds() noexcept {
    keybinds.emplace_back(std::vector<KeyboardKey>{KEY_LEFT_CONTROL, KEY_O},
        openStateFromFile);
    keybinds.emplace_back(std::vector<KeyboardKey>{KEY_LEFT_CONTROL, KEY_S},
        saveStateToFile);
}