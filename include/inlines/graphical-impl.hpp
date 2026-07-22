#ifndef PSIM_GRAPHICAL_IMPL_HPP
#define PSIM_GRAPHICAL_IMPL_HPP

#include "graphical.hpp"
#include "imgui.h"
#include <string>

namespace PSimImpl {
constexpr int GUIWidth = 300;
constexpr int GUIHeight = 720;
constexpr ImVec2 imGuiDefaultCellPadding(5, 5);
constexpr ImVec2 buttonAlignImVec(0.5, 0.5);
constexpr ImVec2 particleCreationSize(GUIWidth - PSimImpl::imGuiDefaultCellPadding.x * 3, 260);
constexpr ImVec2 particleCreationButtonSize(80, 40);
constexpr ImVec2 constantsMenuSize(GUIWidth - PSimImpl::imGuiDefaultCellPadding.x * 3, 100);
constexpr ImVec2 constantInputSize(100, 30);
constexpr ImVec2 constantButtonSize(100, 20);
constexpr ImVec4 appVersionColorRGBA(0.6f, 0.6f, 0.6f, 1.0f);
constexpr ImVec4 selectableColorRGBA(0.26f, 0.59f, 0.98f, 0.40f);
constexpr ImVec2 moduleListElementSize{100, 30};
constexpr int modeToggleHeaderPadding = 10;
constexpr const char* repoURL = "https://github.com/etaiami09-cmd/PSimUltimate";
constexpr const char* repoIssuesURL = "https://github.com/etaiami09-cmd/PSimUltimate/issues/new";
constexpr const char* stardancePage = "https://stardance.hackclub.com/projects/15818";
constexpr std::string settingsPageImGuiID{"Settings"};
constexpr ImVec2 settingsPageSize{800, 400};
constexpr ImVec2 settingsPageMenuSize{200, 0};
constexpr ImVec2 settingsPageContentSize{0, 0};
constexpr ImVec2 sliderSize{300, constantButtonSize.y};
constexpr ImVec2 moduleSwitchPadding{10, 10};
} // namespace PSimImpl

#endif