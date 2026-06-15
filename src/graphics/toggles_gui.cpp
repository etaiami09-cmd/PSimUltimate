#include <string>
#include <format>

#include "toggles_gui.hpp"
#include "graphical.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "simulation.hpp"
#include "holder.hpp"
#include "particles.hpp"
#include "simulation.hpp"

namespace {
constexpr ImVec2 modeToggleButtonSize(90, 30);
constexpr int modeButtonFontSize = 19;

void drawSimulationPauseGUI() {
    std::string buttonText = Simulation::on() ? "Disable" : "Enabled";
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", std::format("Simulation: {}", Simulation::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    StyleVarHolder holder(ImGuiStyleVar_ButtonTextAlign, PSimImpl::buttonAlignImVec);
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - modeToggleButtonSize.x
    );
    if (ImGui::Button(std::format("{}##simulation_button", buttonText).c_str(), modeToggleButtonSize)) {
        Simulation::toggle();
    }
}

void drawElectricToggleGUI() {
    std::string buttonText = Electric::on() ? "Disable" : "Enable";
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", std::format("Electric: {}", Electric::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    StyleVarHolder holder(ImGuiStyleVar_ButtonTextAlign, PSimImpl::buttonAlignImVec);
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - modeToggleButtonSize.x
    );
    if (ImGui::Button(std::format("{}##electric_button", buttonText).c_str(), modeToggleButtonSize)) {
        Electric::toggle();
    }
}

void drawGravityToggleGUI() {
    std::string buttonText = Gravity::on() ? "Disable" : "Enable";
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", std::format("Gravity: {}", Gravity::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    StyleVarHolder holder(ImGuiStyleVar_ButtonTextAlign, PSimImpl::buttonAlignImVec);
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - modeToggleButtonSize.x
    );
    if (ImGui::Button(std::format("{}##gravity_button", buttonText).c_str(), modeToggleButtonSize)) {
        Gravity::toggle();
    }
}
} // namespace

void drawModeToggleGUI() {
    ImGui::SeparatorText("Toggle Modes");
    ImGui::Spacing();
    GroupHolder group;
    drawSimulationPauseGUI();
    drawElectricToggleGUI();
    drawGravityToggleGUI();
}