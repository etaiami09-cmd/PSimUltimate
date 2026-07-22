#include "module_gui.hpp"
#include "particles.hpp"
#include "file_save_open.hpp"
#include "imgui.h"
#include "open_shell_url.hpp"
#include "graphical.hpp"

#include "menu_bar.hpp"

#include <format>
#include <map>
#include <set>

#include "settings_menu.hpp"

namespace {

std::map<std::string, std::map<std::string, std::function<void()>>> moduleButtons;

void drawFileMenu() {
    if (ImGui::BeginMenu("File")) {        
        ImGui::SeparatorText("Simulation Files");
        if (ImGui::MenuItem("Open Simulation", "Ctrl+O")) {
            openStateFromFile();
        }
        if (ImGui::MenuItem("Save Simulation", "Crtl+S")) {
            saveStateToFile();
        }
        ImGui::SeparatorText("Modules");
        if (ImGui::MenuItem("Open Module", "Ctrl+M")) {
            loadModuleWithDialog();
        }
        ImGui::SeparatorText("Settings");
        if (ImGui::MenuItem("Open Settings")) {
            openSettingsMenu();
        }
        ImGui::SeparatorText("Help");
        if (ImGui::MenuItem("Visit GitHub")) {
            openShellURL(PSimImpl::repoURL);
        }
        if (ImGui::MenuItem("Report A Bug")) {
            openShellURL(PSimImpl::repoIssuesURL);
        }
        ImGui::EndMenu();
    }
}
void drawEditMenu() {
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Clear Simulation")) {
            Particles::reset();
        }
        ImGui::EndMenu();
    }
}
void drawModuleButtons() {
    if (ImGui::BeginMenu("Modules")) {
        for (const auto& [module, buttons] : moduleButtons) {
            ImGui::SeparatorText(module.c_str());
            for (const auto& [buttonName, callback] : buttons) {
                if (ImGui::MenuItem(std::format("{}##_button_{}", buttonName, module).c_str())) {
                    callback();
                }
            }
        }
        ImGui::EndMenu();
    }
}
} // namespace

void drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        drawFileMenu();
        drawEditMenu();
        drawModuleButtons();
        ImGui::EndMainMenuBar();
    }
}

void addModuleTopMenuButton(const std::string &module, const std::string &name, std::function<void()> callback) {
    moduleButtons[module][name] = callback;
}

void removeModuleTopMenuButtons(const std::string& module) noexcept {
	std::erase_if(moduleButtons, [&module](const auto& moduleButtonsPair) {
		return moduleButtonsPair.first == module;
	});
}
