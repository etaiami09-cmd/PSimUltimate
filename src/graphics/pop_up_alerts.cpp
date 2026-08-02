#include <vector>
#include <format>

#include "configs.hpp"
#include "imgui.h"
#include "raylib.h"

#include "pop_up_alerts.hpp"

namespace {

std::vector<std::string> popUps;

constexpr ImVec2 popUpShape{300, 100};
int maxPopUps = -1;
constexpr int defaultMaxPopUps = 3;
constexpr float popUpAlpha = 0.6f;
constexpr ImVec2 bottomLeftPivot{1.0f, 1.0f};
constexpr float popUpPadding = 5.0f;

bool dummyTrue = true;

} // namespace

void pushPopUpAlert(const std::string& text) {
    popUps.emplace_back(text);
}

void displayPopups() {
    size_t displayCount = popUps.size() > maxPopUps ? maxPopUps : popUps.size();
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
                            | ImGuiWindowFlags_AlwaysAutoResize
                            | ImGuiWindowFlags_NoSavedSettings
                            | ImGuiViewportFlags_NoFocusOnAppearing
                            | ImGuiItemFlags_NoNav;
    size_t removeIndex = -1;
    for (size_t i = 0; i < displayCount; i++) {
        ImGui::SetNextWindowPos(ImVec2(static_cast<float>(GetScreenWidth())
            - popUpPadding,
            static_cast<float>(GetScreenHeight()) - static_cast<float>(i) *
            (popUpShape.y + popUpPadding)
            - popUpPadding),
            ImGuiCond_Appearing, bottomLeftPivot);
        ImGui::SetNextWindowSize(popUpShape);
        ImGui::SetNextWindowBgAlpha(popUpAlpha);
        if (ImGui::Begin(std::format("Alert#{}", i).c_str(), &dummyTrue, flags)) {
            ImGui::TextWrapped("%s", popUps[i].c_str());
            if (ImGui::Button("Close")) {
                removeIndex = i;
            }

        }
        ImGui::End();
    }
    if (removeIndex != static_cast<size_t>(-1)) {
        popUps.erase(popUps.begin() + removeIndex);
    }
}

int getMaxNotifications() {
	if (maxPopUps == -1) {
		auto optionalMax = getConfigValue<int>("maxPopUps");
		if (!optionalMax.has_value()) {
			maxPopUps = defaultMaxPopUps;
			setConfigValue<int>("maxPopUps", maxPopUps);
		}
		else {
			maxPopUps = std::get<0>(*optionalMax);
		}
	}
	return maxPopUps;
}

void setMaxNotifications(int newMax) {
	maxPopUps = newMax;
	setConfigValue<int>("maxPopUps", maxPopUps);
}