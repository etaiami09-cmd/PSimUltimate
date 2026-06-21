#include <string>

#include "imgui.h"
#include "load_module.hpp"
#include "portable-file-dialogs.h"

#include "module_gui.hpp"

namespace {
#if defined(_WIN32)
const auto fileTypes{"*.dll"};
#elif defined(__APPLE__)
const auto std::string fileTypes{"*.dylib"};
#else
const auto std::string fileTypes{"*.so"};
#endif
constexpr ImVec2 moduleImportButtonSize(200, 30);
} // namespace

void drawModuleGUI() {
    ImGui::SeparatorText("Modules");
    ImGui::Spacing();
    if (ImGui::Button("Import Module", moduleImportButtonSize)) {
        loadModuleWithDialog();
    }
}

void loadModuleWithDialog() {
    auto fileDialog = pfd::open_file(
        "Open Module",
        "",
        {"Dynamically Linked Libraries", fileTypes}
    );
    auto selection = fileDialog.result();
    if (!selection.empty()) {
        loadModule(selection[0]);
    }
}