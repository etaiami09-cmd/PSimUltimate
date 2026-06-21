#include <format>

#include "graphical-impl.hpp"
#include "imgui.h"
#include "graphical.hpp"
#include "version.hpp"
#include "open_shell_url.hpp"

#include "about_window.hpp"

void drawAboutWindow() {
    ImGui::SeparatorText("About");
    ImGui::Text("PSim Ultimate");
    ImGui::TextColored(PSimImpl::appVersionColorRGBA,
        "%s", std::format("Version {}", PSimImpl::PSimAppVersion_).c_str());
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("© 2026 etaiami09-cmd. MIT License.");
    ImGui::Spacing();

    ImGui::Text("Project Homepage:");
    ImGui::SameLine();
    
    if (ImGui::TextLink("Visit GitHub")) {
        openShellURL(PSimImpl::repoURL);
    }

    ImGui::Text("Report A Bug:");
    ImGui::SameLine();
    if (ImGui::TextLink("Click")) {
        openShellURL(PSimImpl::repoIssuesURL);
    }
}