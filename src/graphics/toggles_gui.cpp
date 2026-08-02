#include <string>
#include <format>

#include "toggles_gui.hpp"
#include "graphical-impl.hpp"
#include "graphical.hpp"
#include "imgui.h"
#include "simulation.hpp"
#include "holder.hpp"
#include "module.hpp"

namespace {
constexpr ImVec2 modeToggleButtonSize(90, 30);

void drawSimulationPauseGUI() {
    std::string buttonText = Simulation::on() ? "Disable" : "Enable";
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

void drawModuleToggleGUI(Module& module) {
    std::string buttonText = module.active.value() ? "Disable" : "Enable";
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", std::format("{}: {}", module.name, module.active.value() ? "On" : "Off").c_str());
    ImGui::SameLine();
    StyleVarHolder holder(ImGuiStyleVar_ButtonTextAlign, PSimImpl::buttonAlignImVec);
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - modeToggleButtonSize.x
    );
    if (ImGui::Button(std::format("{}##{}_button", buttonText, module.name).c_str(),
    modeToggleButtonSize)) {
        module.toggle();
    }
}

} // namespace

void drawModeToggleGUI() {
    ImGui::SeparatorText("Toggle Modes");
    ImGui::Spacing();
    GroupHolder group;
    drawSimulationPauseGUI();
    for (auto& module : getModules()) {
    	if (module.active.has_value()) {
    		drawModuleToggleGUI(module);
    	}
    }
}