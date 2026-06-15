#include <format>

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
float newParticleCharge = 0;
float newParticleMass = 1;

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
void valueInput(const char* name, float* target) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(name);
    ImGui::InputFloat(std::format("##{}_constant", name).c_str(), target);
}
} // namespace

void drawParticleCreationGUI() {
    ImGui::SeparatorText("New Particle");
    ImGui::Spacing();
    ImGui::BeginGroup();
    ImGui::Text("Size");
    ImGui::InputFloat("Radius", &newParticleRadius);
    vecInput("Position", &newParticleX, &newParticleY);
    vecInput("Velocity", &newParticleVelX, &newParticleVelY);
    valueInput("Mass", &newParticleMass);
    valueInput("Charge", &newParticleCharge);
    ImGui::Text("Create Particle");
    if (ImGui::Button("Create", PSimImpl::particleCreationButtonSize)) {
        Particles::add(
            Position{static_cast<float>(newParticleX), static_cast<float>(newParticleY)},
            Velocity{static_cast<float>(newParticleVelX), static_cast<float>(newParticleVelY)},
            newParticleRadius, newParticleMass
        );
        Electric::set(Particles::get().size() - 1, newParticleCharge);
    }
    ImGui::EndGroup();
}