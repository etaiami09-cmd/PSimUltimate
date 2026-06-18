#include <format>
#include <string>
#include <algorithm>

#include "imgui.h"
#include "graphical.hpp"
#include "constants.hpp"

#include "constants_gui.hpp"

namespace {

void drawConstantGUI(const std::string& name, Constant& constant) {
    ImGui::SetNextItemWidth(PSimImpl::constantInputSize.x);
    if (ImGui::InputFloat(std::format("##{}_constant_input", name).c_str(), &constant.value)) {
        constant.buffer = std::clamp(constant.buffer, constant.minValue, constant.maxValue);
    }
    ImGui::SameLine();
    if (ImGui::Button("Set", ImVec2{PSimImpl::constantButtonSize.x, ImGui::GetFrameHeight()})) {
        constant.value = constant.buffer;
        constant.onChange(constant.value);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - ImGui::CalcTextSize(name.c_str()).x
    );
    ImGui::TextUnformatted(name.c_str());
}
} // namespace

void drawConstantsGUI() {
    ImGui::SeparatorText("Edit Constants");
    ImGui::Spacing();
    for (auto& pair : getAllConstants()) {
        drawConstantGUI(pair.first, pair.second);
    }
}