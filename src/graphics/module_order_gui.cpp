#include "module_order_gui.hpp"

#include <format>

#include "graphical-impl.hpp"
#include "holder.hpp"
#include "imgui.h"
#include "module.hpp"

void drawModuleOrderGUI() {
    ImGui::Spacing();
    StyleVarHolder holder{ImGuiStyleVar_SelectableTextAlign, ImVec2{0.5f, 0.5f}};
    ImGui::TextUnformatted("Adjust Module Order");
    ImGui::Spacing();
    size_t moveFrom = std::numeric_limits<size_t>::max();
    size_t moveTo = std::numeric_limits<size_t>::max();
    for (size_t i = 0; i < getModuleIndexOrders().size(); i++) {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::PushID(static_cast<int>(i));
        ImGui::GetWindowDrawList()->AddRectFilled(
            pos,
            ImVec2(pos.x + PSimImpl::moduleListElementSize.x, pos.y + PSimImpl::moduleListElementSize.y),
            ImGui::ColorConvertFloat4ToU32(PSimImpl::selectableColorRGBA)
        );
        ImGui::Selectable(
            getModules()[*std::next(getModuleIndexOrders().begin(), static_cast<ssize_t>(i))]
            .name.c_str(),
            false,
            ImGuiSelectableFlags_None,
            PSimImpl::moduleListElementSize);

        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("MODULE_ORDER_ITEM", &i, sizeof(i));
            ImGui::Text("%s",
                std::format("Moving: {}",
                    getModules()[*std::next(getModuleIndexOrders().begin(), static_cast<ssize_t>(i))]
                    .name).c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODULE_ORDER_ITEM")) {
                moveFrom = *static_cast<const size_t*>(payload->Data);
                moveTo = i;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
    }

    if (moveFrom != std::numeric_limits<size_t>::max()
        && moveTo != std::numeric_limits<size_t>::max()) {
        repositionModule(moveFrom, moveTo);
    }
}
