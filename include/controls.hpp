#ifndef PSIM_CONTROLS_HPP
#define PSIM_CONTROLS_HPP

#include <vector>
#include <string>
#include <functional>
#include <format>

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

    std::string formatKeys() const noexcept;
};

void handleControls();

void registerKeybinds() noexcept;

void addKeybind(const std::string& module, const std::string& name,
    std::span<const KeyboardKey> keys, std::span<const MouseButton> buttons,
    const std::function<void()>& callback) noexcept;

std::span<Keybind> getKeybinds();

template<>
struct std::formatter<KeyboardKey> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto iterator = ctx.begin();
        if (iterator != ctx.end() && *iterator != '}') {
            throw std::format_error("Invalid format specifier for KeyboardKey");
        }
        return iterator;
    }

    auto format(KeyboardKey key, std::format_context& ctx) const {
        std::string result;
        static const std::unordered_map<KeyboardKey, std::string_view> names = {
            {KEY_NULL, "None"},
            {KEY_ESCAPE, "Esc"},
            {KEY_ENTER, "Enter"},
            {KEY_TAB, "Tab"},
            {KEY_BACKSPACE, "Backspace"},
            {KEY_INSERT, "Insert"},
            {KEY_DELETE, "Delete"},
            {KEY_RIGHT, "Right"},
            {KEY_LEFT, "Left"},
            {KEY_UP, "Up"},
            {KEY_DOWN, "Down"},
            {KEY_PAGE_DOWN, "PgDn"},
            {KEY_PAGE_UP, "PgUp"},
            {KEY_HOME, "Home"},
            {KEY_END, "End"},
            {KEY_CAPS_LOCK, "CapsLock"},
            {KEY_LEFT_SHIFT, "LShift"},
            {KEY_RIGHT_SHIFT, "RShift"},
            {KEY_LEFT_CONTROL, "LCtrl"},
            {KEY_RIGHT_CONTROL, "RCtrl"},
            {KEY_LEFT_ALT, "LAlt"},
            {KEY_RIGHT_ALT, "RAlt"},
            {KEY_F1, "F1"}, {KEY_F2, "F2"},
            {KEY_F3, "F3"}, {KEY_F4, "F4"},
            {KEY_F5, "F5"}, {KEY_F6, "F6"},
            {KEY_F7, "F7"}, {KEY_F8, "F8"},
            {KEY_F9, "F9"}, {KEY_F10, "F10"},
            {KEY_F11, "F11"}, {KEY_F12, "F12"},
            {KEY_KP_0, "Num0"}, {KEY_KP_1, "Num1"},
            {KEY_KP_2, "Num2"}, {KEY_KP_3, "Num3"},
            {KEY_KP_4, "Num4"}, {KEY_KP_5, "Num5"},
            {KEY_KP_6, "Num6"}, {KEY_KP_7, "Num7"},
            {KEY_KP_8, "Num8"}, {KEY_KP_9, "Num9"},
            {KEY_KP_DECIMAL, "Num."}, {KEY_KP_ADD, "Num+"},
            {KEY_KP_SUBTRACT, "Num-"}, {KEY_KP_MULTIPLY, "Num*"},
            {KEY_KP_DIVIDE, "Num/"}, {KEY_KP_ENTER, "NumEnter"}
        };
        if (key >= 32 && key <= 126) {
            result = static_cast<char>(key);
        }
        else {
            auto iterator = names.find(key);
            result = (iterator != names.end()) ? iterator->second : "?";
        }
        return std::format_to(ctx.out(), "{}", result);
    }
};

template<>
struct std::formatter<MouseButton> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto iterator = ctx.begin();
        if (iterator != ctx.end() && *iterator != '}') {
            throw std::format_error("Invalid format specifier for MouseButton");
        }
        return iterator;
    }

    auto format(MouseButton button, std::format_context& ctx) const {
        std::string result;
        switch (button) {
            case MOUSE_BUTTON_LEFT: result = "LMB"; break;
            case MOUSE_BUTTON_RIGHT: result = "RMB"; break;
            case MOUSE_BUTTON_MIDDLE: result = "MMB"; break;
            default: result = "MB"; break;
        }
        return std::format_to(ctx.out(), "{}", result);
    }
};

#endif