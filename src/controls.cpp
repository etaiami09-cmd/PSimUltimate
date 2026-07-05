#include <vector>
#include <algorithm>
#include <functional>

#include "raylib.h"
#include "portable-file-dialogs.h" // IWYU pragma: export
#include "file_save_open.hpp"

#include "controls.hpp"

#include "module.hpp"

namespace {
std::vector<Keybind> keybinds;

bool keybindModuleActive(const Keybind& keybind) {
    return keybind.module == "internal_keybind"
        || isModuleActive((keybind.module));
}

bool isKeybindActivated(const Keybind& keybind) {
    return keybindModuleActive(keybind)
        && (std::ranges::all_of(keybind.keys.begin(), keybind.keys.end(),
        IsKeyDown)
        && std::ranges::all_of(keybind.buttons.begin(), keybind.buttons.end(),
        IsMouseButtonDown));
}
} // namespace

Keybind::Keybind(const std::string& module, const std::string& name,
    std::span<const KeyboardKey> keys, std::span<const MouseButton> buttons,
    const std::function<void()>& callback) noexcept
: keys(keys.begin(),keys.end()), buttons(buttons.begin(),
    buttons.end()), module(module), name(name), callback(callback) {}

void handleControls() {
    for (const auto& keybind : keybinds) {
        if (isKeybindActivated(keybind))
        {
            keybind.callback();
        }
    }
}

void registerKeybinds() noexcept {
    std::vector<KeyboardKey> openFile{KEY_LEFT_CONTROL, KEY_O};
    keybinds.emplace_back("internal_keybind", "Open Simulation",
        openFile, std::span<const MouseButton>{}, openStateFromFile);
    std::vector<KeyboardKey> saveFile{KEY_LEFT_CONTROL, KEY_S};
    keybinds.emplace_back("internal_keybind", "Save Simulation",
        saveFile, std::span<const MouseButton>{}, saveStateToFile);
}

void addKeybind(const std::string &module, const std::string& name,
    std::span<const KeyboardKey> keys, std::span<const MouseButton> buttons,
    const std::function<void()> &callback) noexcept {
    keybinds.emplace_back(module, name, keys, buttons, callback);
}