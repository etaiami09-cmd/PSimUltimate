#ifndef PSIM_STYLE_VAR_HOLDER_HPP
#define PSIM_STYLE_VAR_HOLDER_HPP

#include "imgui.h"

struct StyleVarHolder {
    StyleVarHolder(ImGuiStyleVar idx, const ImVec2& val);
    ~StyleVarHolder();
};

#endif