#ifndef PSIM_HOLDER_HPP
#define PSIM_HOLDER_HPP

#include "imgui.h"
#include <utility>

template<auto Push, auto Pop>
struct Holder {
    template<typename... Args>
    explicit Holder(Args&&... args) {
        Push(std::forward<Args>(args)...);
    }
    ~Holder() {
        Pop();
    }
};


using ChildHolder = Holder<
[](const char* str_id, const ImVec2& size=ImVec2(0, 0),
    ImGuiChildFlags child_flags=0, ImGuiWindowFlags window_flags=0) {
    ImGui::BeginChild(str_id, size, child_flags, window_flags);
}, ImGui::EndChild>;

using StyleVarHolder = Holder<
[](ImGuiStyleVar idx, const ImVec2& val) {
    ImGui::PushStyleVar(idx, val);
}, [](){ImGui::PopStyleVar();}>;

using StyleColorHolder = Holder<
    [](ImGuiCol idx, const ImVec4& col) {
        ImGui::PushStyleColor(idx, col);
}, [](){ImGui::PopStyleColor();}>;

using GroupHolder = Holder<ImGui::BeginGroup, ImGui::EndGroup>;

#endif