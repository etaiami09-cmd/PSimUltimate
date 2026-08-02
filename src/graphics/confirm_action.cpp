#include <format>
#include <imgui.h>

#include "pop_up_alerts.hpp"

#include "confirm_action.hpp"

namespace {

struct ActionConfirmation {
	std::string actionName;
	std::string explanation;
	std::function<void()> action;
	int actionId;

	ActionConfirmation(const std::string& actionName, const std::string& explanation,
		std::function<void()> action)
	: actionName(actionName), explanation(explanation), action(action) {
		static int counter = 0;
		actionId = counter++;
	}
};

std::vector<ActionConfirmation> actions;
bool needNewPopup = false;

} // namespace

void confirmAction(const std::string& actionName, const std::string& explanation, std::function<void()> action) {
	actions.emplace_back(actionName, explanation, action);
	needNewPopup = true;
}

void drawConfirmationPopup() {
	if (actions.empty()) {return;};
	if (needNewPopup) {
		ImGui::OpenPopup(std::format("Confirm Action##{}",
			actions[actions.size() - 1].actionId).c_str());
		needNewPopup = false;
	}
	const auto& action = actions[0];
	if (ImGui::BeginPopupModal(std::format("Confirm Action##{}",
			action.actionId).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::TextUnformatted(action.explanation.c_str());
		if (ImGui::Button(action.actionName.c_str(), {0, 0})) {
			action.action();
			actions.erase(actions.begin());
			ImGui::CloseCurrentPopup();
		}
		else if (ImGui::SameLine(); ImGui::Button("Cancel", {0, 0})) {
			actions.erase(actions.begin());
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}