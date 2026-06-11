#include "style_var_holder.hpp"
#include "imgui.h"

StyleVarHolder::StyleVarHolder(ImGuiStyleVar idx, const ImVec2& val) {
    ImGui::PushStyleVar(idx, val);
}

StyleVarHolder::~StyleVarHolder() {
    ImGui::PopStyleVar();
}