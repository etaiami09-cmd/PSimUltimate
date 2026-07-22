#include <format>
#include <vector>
#include <cstddef>
#include <algorithm>

#include "attributes.hpp"
#include "configs.hpp"
#include "imgui.h"
#include "graphical.hpp"
#include "particles.hpp"
#include "Particle.hpp"
#include "raylib.h"

#include "particle_creation_gui.hpp"

namespace
{
constexpr int defaultRadius = 10;
int newParticleX = 0;
int newParticleY = 0;
int newParticleVelX = 0;
int newParticleVelY = 0;
float newParticleRadius = defaultRadius;
float newParticleMass = 1;
std::vector<float> newParticleAttributes;

void vecInput(const char* name, int* xTarget, int* yTarget) {
	ImGui::TextUnformatted(name);
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::InputInt(std::format("##{}_x", name).c_str(), xTarget);
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::InputInt(std::format("##{}_y", name).c_str(), yTarget);
}

void valueInput(const char* name, float* target, float minValue, float maxValue) {
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(name);
	if (ImGui::InputFloat(std::format("##{}_constant", name).c_str(), target)) {
		*target = std::clamp(*target, minValue, maxValue);
	}
}

void drawTargetPosition() {
	bool shouldDraw = shouldDrawTarget();
	if (!shouldDraw) { return; }
	std::array<float, 4> particleTargetColor = getNewParticleTargetColor();
	Color raylibTargetColor = ColorFromNormalized(Vector4{
		particleTargetColor[0], particleTargetColor[1],
		particleTargetColor[2], particleTargetColor[3]
	});
	DrawLineEx(Vector2{
		           newParticleX - (getNewParticleTargetLength() / 2) + getPSimGUIWidth(),
		           newParticleY + ImGui::GetFrameHeight()
	           },
	           Vector2{
		           newParticleX + (getNewParticleTargetLength() / 2) + getPSimGUIWidth(),
		           newParticleY + ImGui::GetFrameHeight()
	           },
	           getNewParticleTargetWidth(), raylibTargetColor);
	DrawLineEx(Vector2{
		           newParticleX + getPSimGUIWidth(),
		           newParticleY - (getNewParticleTargetLength() / 2) + ImGui::GetFrameHeight()
	           },
	           Vector2{
		           newParticleX + getPSimGUIWidth(),
		           newParticleY + (getNewParticleTargetLength() / 2) + ImGui::GetFrameHeight()
	           },
	           getNewParticleTargetWidth(), raylibTargetColor);
}
} // namespace

void drawParticleCreationGUI() {
	newParticleAttributes.resize(getAttributes().size());
	ImGui::SeparatorText("New Particle");
	ImGui::Spacing();
	ImGui::BeginGroup();
	vecInput("Position", &newParticleX, &newParticleY);
	vecInput("Velocity", &newParticleVelX, &newParticleVelY);
	valueInput("Radius", &newParticleRadius, Particle::minRadius, Particle::maxRadius);
	valueInput("Mass", &newParticleMass, Particle::minMass, Particle::maxMass);
	for (size_t i = 0; i < newParticleAttributes.size(); i++) {
		const auto& attribute = getAttributes()[i];
		valueInput(attribute.name.c_str(), &newParticleAttributes[i], attribute.minValue, attribute.maxValue);
	}
	ImGui::Text("Create Particle");
	if (ImGui::Button("Create", PSimImpl::particleCreationButtonSize)) {
		Particles::add(
			Position{static_cast<float>(newParticleX), static_cast<float>(newParticleY)},
			Velocity{static_cast<float>(newParticleVelX), static_cast<float>(newParticleVelY)},
			newParticleRadius, newParticleMass,
			newParticleAttributes
		);
	}
	ImGui::EndGroup();
	drawTargetPosition();
}

namespace
{
constexpr bool defaultShouldDrawTargetValue = true;
bool shouldDrawTargetValue;
bool shouldDrawTargetHasValue = false;
} // namespace

bool shouldDrawTarget() {
	if (!shouldDrawTargetHasValue) {
		auto configValue = getConfigValue<bool>("ShouldDrawTarget");
		if (!configValue.has_value()) {
			shouldDrawTargetValue = defaultShouldDrawTargetValue;
			setConfigValue<bool>("ShouldDrawTarget", shouldDrawTargetValue);
		}
		else {
			shouldDrawTargetValue = std::get<0>(configValue.value());
		}
	}
	return shouldDrawTargetValue;
}

void setShouldDrawTarget(bool newValue) {
	shouldDrawTargetValue = newValue;
	setConfigValue<bool>("ShouldDrawTarget", shouldDrawTargetValue);
}

namespace {

std::array<float, 4> targetColor = {-1.0f, -1.0f, -1.0f, -1.0f};
constexpr std::array<float, 4> defaultTargetColor = {1.0f, 0.0f, 0.0f, 1.0f};

int targetLength = -1;
constexpr int defaultTargetLength = 10;

int targetWidth = -1;
constexpr int defaultTargetWidth = 10;

} // namespace

std::array<float, 4> getNewParticleTargetColor() {
	if (targetColor[0] == -1.0f) {
		auto configValue = getConfigValue<decltype(targetColor)>("ParticleTargetColor");
		if (!configValue.has_value()) {
			targetColor = defaultTargetColor;
			setConfigValue<decltype(targetColor)>("ParticleTargetColor", targetColor);
		}
		else {
			targetColor = std::get<0>(configValue.value());
		}
	}
	return targetColor;
}
void setNewParticleTargetColor(std::array<float, 4> newColor) {
	targetColor = newColor;
	setConfigValue<decltype(targetColor)>("ParticleTargetColor", targetColor);
}
int getNewParticleTargetLength() {
	if (targetLength == -1) {
		auto configValue = getConfigValue<int>("ParticleTargetLength");
		if (!configValue.has_value()) {
			targetLength = defaultTargetLength;
			setConfigValue<int>("ParticleTargetLength", targetLength);
		}
		else {
			targetLength = std::get<0>(configValue.value());
		}
	}
	return targetLength;
}
void setNewParticleTargetLength(int newLength) {
	targetLength = newLength;
	setConfigValue<int>("ParticleTargetLength", newLength);
}
int getNewParticleTargetWidth() {
	if (targetWidth == -1) {
		auto configValue = getConfigValue<int>("ParticleTargetWidth");
		if (!configValue.has_value()) {
			targetWidth = defaultTargetWidth;
			setConfigValue<int>("ParticleTargetWidth", targetWidth);
		}
		else {
			targetWidth = std::get<0>(configValue.value());
		}
	}
	return targetWidth;
}
void setNewParticleTargetWidth(int newWidth) {
	targetWidth = newWidth;
	setConfigValue<int>("ParticleTargetWidth", newWidth);
}
