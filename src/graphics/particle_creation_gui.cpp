#include <format>
#include <vector>
#include <cstddef>
#include <algorithm>

#include "attributes.hpp"
#include "imgui.h"
#include "graphical.hpp"
#include "particles.hpp"
#include "Particle.hpp"

#include "particle_creation_gui.hpp"

namespace {
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
}