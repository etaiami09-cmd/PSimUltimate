#ifndef PSIM_MENU_BAR_HPP
#define PSIM_MENU_BAR_HPP

#include <functional>
#include <string>

void drawMenuBar();

void addModuleTopMenuButton(const std::string& module, const std::string& name, std::function<void()> callback);
void removeModuleTopMenuButtons(const std::string& module) noexcept;

#endif