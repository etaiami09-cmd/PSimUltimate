#include "graphical-impl.hpp"
#include "module_gui.hpp"
#include "file_save_open.hpp"
#include "imgui.h"
#include "open_shell_url.hpp"
#include "graphical.hpp"

#include "menu_bar.hpp"

void drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
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
        ImGui::EndMainMenuBar();
    }
}