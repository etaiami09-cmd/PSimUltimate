#ifndef PSIM_SETTINGS_MENU_HPP
#define PSIM_SETTINGS_MENU_HPP

#include <functional>

void openSettingsMenu();

void renderSettingsModalIfOpened();

void addModuleSwitch(const std::string& module, const std::string& name, bool defaultValue,
    const std::function<void(bool)>& onChange);

void removeModuleSwitches(const std::string& module) noexcept;

#endif