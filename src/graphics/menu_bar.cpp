#include "module_gui.hpp"
#include "particles.hpp"
#include "file_save_open.hpp"
#include "imgui.h"
#include "open_shell_url.hpp"
#include "graphical.hpp"

#include "menu_bar.hpp"

namespace {
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
        if (ImGui::MenuItem("Open Module")) {
            loadModuleWithDialog();
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
} // namespace

void drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        drawFileMenu();
        drawEditMenu();
        ImGui::EndMainMenuBar();
    }
}