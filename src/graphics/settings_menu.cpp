#include <array>

#include "imgui.h"
#include "graphical-impl.hpp"

#include "settings_menu.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <version.hpp>

#include "controls.hpp"
#include "fps.hpp"
#include "holder.hpp"
#include "particles.hpp"
#include "particle_creation_gui.hpp"
#include "pop_up_alerts.hpp"

namespace {
bool firstSettingsFrame = true;
bool openMenu = false;
bool keepMenuOpen = false;

struct ModuleSwitch {
    std::string name;
    bool value;
    std::function<void(bool)> onChange;
};

size_t selectedTab = 0;
constexpr auto tabs = std::array{
    "General##SettingsTab", "Simulation##SettingsTab",
    "Modules##SettingsTab", "About##SettingsTab"
};
std::map<std::string, std::vector<ModuleSwitch> > moduleSwitches;

void drawTabMenu() {
    ImGui::BeginChild("##SettingsNavMenu", PSimImpl::settingsPageMenuSize,
                      ImGuiChildFlags_Borders);
    for (size_t i = 0; i < tabs.size(); i++) {
        if (ImGui::Selectable(tabs[i], selectedTab == i)) {
            selectedTab = i;
        }
    }
    ImGui::EndChild();
}

void drawKeybindsTable() {
    ImGui::SeparatorText("Keybinds");
    ImGui::Spacing();
    if (ImGui::BeginTable("KeybindsTable", 3, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Module");
        ImGui::TableSetupColumn("Keybind");
        ImVec4 header_bg = ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, header_bg);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, header_bg);
        ImGui::TableHeadersRow();
        for (const auto &keybind: getKeybinds()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(keybind.name.c_str());
            ImGui::TableNextColumn();
            std::string module = keybind.module;
            module = (module == "internal_keybind") ? "" : module;
            ImGui::TextUnformatted(module.c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(keybind.formatKeys().c_str());
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::EndTable();
    }
}

int fpsBuffer = -1;

void drawFPSSlider() {
    if (fpsBuffer == -1) {
        fpsBuffer = getFPS();
    }
    ImGui::SetNextItemWidth(PSimImpl::sliderSize.x);
    ImGui::SliderInt("##FPSSlider", &fpsBuffer, 5, 240,
                     std::format("{} FPS", fpsBuffer).c_str());
    fpsBuffer = (fpsBuffer / 5) * 5;
    ImGui::BeginDisabled(fpsBuffer == getFPS());
    ImGui::SameLine();
    if (ImGui::Button("Set", ImVec2{PSimImpl::constantButtonSize.x, 0})) {
        setFPS(fpsBuffer);
    }
    ImGui::EndDisabled();
}

int ticksBuffer = -1;

void drawTicksPerFrameSlider() {
    if (ticksBuffer == -1) {
        ticksBuffer = getTicksPerFrame();
    }
    ImGui::SetNextItemWidth(PSimImpl::sliderSize.x);
    ImGui::SliderInt("##TicksPerFrame", &ticksBuffer, 1, 100,
                     std::format("{} Tick{} Per Frame", ticksBuffer,
                                 (ticksBuffer == 1) ? "" : "s").c_str());
    ImGui::BeginDisabled(ticksBuffer == getTicksPerFrame());
    ImGui::SameLine();
    if (ImGui::Button("Set##TicksPerFrame", ImVec2{PSimImpl::constantButtonSize.x, 0})) {
        setTicksPerFrame(ticksBuffer);
    }
    ImGui::EndDisabled();
}

int maxNotificationsBuffer = -1;

void drawMaxNotificationsSlider() {
	if (maxNotificationsBuffer == -1) {
		maxNotificationsBuffer = getMaxNotifications();
	}
	ImGui::SetNextItemWidth(PSimImpl::sliderSize.x);
	ImGui::SliderInt("##MaxNotifications", &maxNotificationsBuffer, 1, 10,
					 std::format("{} Maximum Displayed Notifications",
					 	maxNotificationsBuffer).c_str());
	ImGui::BeginDisabled(maxNotificationsBuffer == getMaxNotifications());
	ImGui::SameLine();
	if (ImGui::Button("Set##MaxNotifications", ImVec2{PSimImpl::constantButtonSize.x, 0})) {
		setMaxNotifications(maxNotificationsBuffer);
	}
	ImGui::EndDisabled();
}

void drawTargetDrawingSection() {
	ImGui::SeparatorText("New Particle Target");
	ImGui::Spacing();
	if (ImGui::Button(std::format("Draw New Particle Position Target: {}",
		shouldDrawTarget() ? "On" : "Off").c_str(), ImVec2{0, 0})) {
		setShouldDrawTarget(!shouldDrawTarget());
	}
	std::array<float, 4> particleTargetColorBuffer = getNewParticleTargetColor();
	if (ImGui::ColorEdit4("Color##TargetColorSelection", particleTargetColorBuffer.data(),
		ImGuiColorEditFlags_NoInputs)) {
		setNewParticleTargetColor(particleTargetColorBuffer);
	}
	ImGui::SetNextItemWidth(PSimImpl::sliderSize.x);
	static int targetLineLengthBuffer = getNewParticleTargetLength();
	ImGui::SliderInt("##TargetLineLength", &targetLineLengthBuffer, 1, 100,
					 std::format("{} Target Line Length",
						 targetLineLengthBuffer).c_str());
	ImGui::BeginDisabled(targetLineLengthBuffer == getNewParticleTargetLength());
	ImGui::SameLine();
	if (ImGui::Button("Set##TargetLineLengthSet", ImVec2{PSimImpl::constantButtonSize.x, 0})) {
		setNewParticleTargetLength(targetLineLengthBuffer);
	}
	ImGui::EndDisabled();
	ImGui::SetNextItemWidth(PSimImpl::sliderSize.x);
	static int targetLineWidthBuffer = getNewParticleTargetWidth();
	ImGui::SliderInt("##TargetLineWidth", &targetLineWidthBuffer, 1, 20,
					 std::format("{} Target Line Width",
						 targetLineWidthBuffer).c_str());
	ImGui::BeginDisabled(targetLineWidthBuffer == getNewParticleTargetWidth());
	ImGui::SameLine();
	if (ImGui::Button("Set##TargetLineWidthSet", ImVec2{PSimImpl::constantButtonSize.x, 0})) {
		setNewParticleTargetWidth(targetLineWidthBuffer);
	}
	ImGui::EndDisabled();
}

void drawGeneralTab() {
    ImGui::TextUnformatted("General");
    ImGui::Separator();
    ImGui::Spacing();
    drawFPSSlider();
	drawMaxNotificationsSlider();
	drawTargetDrawingSection();
    drawKeybindsTable();
}

void drawSimulationTab() {
    ImGui::TextUnformatted("Simulation");
    ImGui::Separator();
    ImGui::Spacing();
    drawTicksPerFrameSlider();
}

void drawModulesTab() {
    ImGui::TextUnformatted("Modules");
    ImGui::Separator();
    ImGui::Spacing();
    StyleVarHolder holder{ImGuiStyleVar_FramePadding, PSimImpl::moduleSwitchPadding};
    for (auto& [module, switches]: moduleSwitches) {
        ImGui::SeparatorText(module.c_str());
        for (auto& moduleSwitch : switches) {
            if (ImGui::Button(std::format("{}: {}", moduleSwitch.name,
                (moduleSwitch.value) ? "On" : "Off").c_str(), ImVec2{0, 0})) {
                moduleSwitch.value = !moduleSwitch.value;
                moduleSwitch.onChange(moduleSwitch.value);
            }
        }
    }
}

void drawAboutTab() {
    ImGui::TextUnformatted("About");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextUnformatted(std::format("PSimUltimate {}", PSimImpl::PSimAppVersion_).c_str());
    ImGui::TextLinkOpenURL("GitHub", "https://github.com/etaiami09-cmd/PSimUltimate");
    ImGui::TextUnformatted(PSimImpl::PSimLicenseText_);
}

void drawSettingsContent() {
    ImGui::SameLine();
    ImGui::BeginChild("##SettingsContent", PSimImpl::settingsPageContentSize,
                      ImGuiChildFlags_Borders);
    switch (selectedTab) {
        case 0: {
            drawGeneralTab();
            break;
        }
        case 1: {
            drawSimulationTab();
            break;
        }
        case 2: {
            drawModulesTab();
            break;
        }
        case 3: {
            drawAboutTab();
            break;
        }
        default: break;
    }
    ImGui::EndChild();
}
} // namespace

void openSettingsMenu() {
    openMenu = true;
    keepMenuOpen = true;
}

void renderSettingsModalIfOpened() {
    if (openMenu) {
        ImGui::OpenPopup(PSimImpl::settingsPageImGuiID.c_str());
        openMenu = false;
    }
    if (firstSettingsFrame) {
        ImGui::SetNextWindowSize(PSimImpl::settingsPageSize);
        firstSettingsFrame = false;
    }
    if (ImGui::BeginPopupModal(PSimImpl::settingsPageImGuiID.c_str(),
                               &keepMenuOpen, ImGuiWindowFlags_None)) {
        drawTabMenu();
        drawSettingsContent();
        if (!keepMenuOpen) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void addModuleSwitch(const std::string &module, const std::string &name, bool defaultValue,
                          const std::function<void(bool)> &onChange) {
    moduleSwitches[module].emplace_back(name, defaultValue, onChange);
    std::ranges::sort(moduleSwitches[module], {}, &ModuleSwitch::name);
}

void removeModuleSwitches(const std::string& module) noexcept {
	std::erase_if(moduleSwitches, [&module](const auto& moduleSwitchesPair) {
		return moduleSwitchesPair.first == module;
	});
}
