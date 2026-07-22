#include <vector>
#include <algorithm>
#include <functional>

#include "raylib.h"
#include "portable-file-dialogs.h" // IWYU pragma: export
#include "file_save_open.hpp"

#include "controls.hpp"

#include "module.hpp"
#include "module_gui.hpp"

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

std::string Keybind::formatKeys() const noexcept {
    std::string result{};
    if (buttons.empty()) {
        for (size_t i = 0; i < keys.size() - 1; i++) {
            result += std::format("{}+", keys[i]);
        }
        result += std::format("{}", keys[keys.size() - 1]);
        return result;
    }
    for (size_t i = 0; i < keys.size(); i++) {
        result += std::format("{}+", keys[i]);
    }
    for (size_t i = 0; i < buttons.size() - 1; i++) {
        result += std::format("{}+", buttons[i]);
    }
    result += std::format("{}", buttons[buttons.size() - 1]);
    return result;
}

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
	std::vector<KeyboardKey> importExtension{KEY_LEFT_CONTROL, KEY_M};
	keybinds.emplace_back("internal_keybind", "Import Module",
		importExtension, std::span<const MouseButton>{}, loadModuleWithDialog);
}

void addKeybind(const std::string &module, const std::string& name,
    std::span<const KeyboardKey> keys, std::span<const MouseButton> buttons,
    const std::function<void()> &callback) noexcept {
    keybinds.emplace_back(module, name, keys, buttons, callback);
}

void removeModuleKeybinds(const std::string& module) noexcept {
	std::erase_if(keybinds, [&module](const auto& keybind) {
		return keybind.module == module;
	});
}

std::span<Keybind> getKeybinds() {
    return keybinds;
}
