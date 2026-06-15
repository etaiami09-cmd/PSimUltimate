#include "imgui.h"
#include "particles.hpp"
#include "graphical.hpp"

#include "constants_gui.hpp"

namespace {
float newGravity = Gravity::getDefault();

void drawGravityConstantGUI() {
    ImGui::SetNextItemWidth(PSimImpl::constantInputSize.x);
    ImGui::InputFloat("##gravity_input", &newGravity);
    ImGui::SameLine();
    if (ImGui::Button("Set", ImVec2{PSimImpl::constantButtonSize.x, ImGui::GetFrameHeight()})) {
        Gravity::set(newGravity);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - ImGui::CalcTextSize("Gravity").x
    );
    ImGui::Text("Gravity");
}
} // namespace

void drawConstantsGUI() {
    ImGui::SeparatorText("Edit Constants");
    ImGui::Spacing();
    drawGravityConstantGUI();
}