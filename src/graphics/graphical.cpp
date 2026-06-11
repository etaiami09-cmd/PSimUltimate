#include <cmath>
#include <string>
#include <cstddef>
#include <format>

#include "graphical.hpp"
#include "simulation.hpp"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "particles.hpp"
#include "Particle.hpp"
#include "open_shell_url.hpp"
#include "style_var_holder.hpp"
#include "version.hpp"

namespace {
constexpr const char* windowTitle = "Particle Simulator Ultimate";
constexpr int windowWidth = 1270;
constexpr int windowHeight = 720;
constexpr int targetFPS = 60;
constexpr int GUIWidth = 300;
constexpr int GUIHeight = 720;
constexpr float textButtonPadding = 5.0;
constexpr int modeButtonFontSize = 19;
constexpr int modeToggleHeaderPadding = 10;
constexpr ImVec2 imGuiDefaultCellPadding(5, 5);
constexpr ImVec2 buttonAlignImVec(0.5, 0.5);
constexpr ImVec2 particleCreationSize(GUIWidth - imGuiDefaultCellPadding.x * 3, 260);
constexpr ImVec2 particleCreationButtonSize(80, 40);
constexpr ImVec2 modeToggleSize(GUIWidth - imGuiDefaultCellPadding.x * 3, 150);
constexpr ImVec2 modeToggleButtonSize(90, 30);
constexpr ImVec4 appVersionColorRGBA(0.6f, 0.6f, 0.6f, 1.0f);
constexpr const char* repoURL = "https://github.com/etaiami09-cmd/PSimUltimate";
} // namespace

void startWindow() {
    SetTraceLogLevel(LOG_NONE);
    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(targetFPS);
    rlImGuiSetup(true);
    ImGui::PushFont(ImGui::GetIO().Fonts->AddFontDefault());
}

void closeWindow() {
    rlImGuiShutdown();
    CloseWindow();
}

namespace {
void toggleElectric() {
    if (Electric::on()) {
        Electric::finish();
    }
    else {
        Electric::init();
    }
}
void toggleGravity() {
    if (Gravity::on()) {
        Gravity::finish();
    }
    else {
        Gravity::init();
    }
}
void toggleSimulation() {
    if (Simulation::on()) {
        Simulation::pause();
    }
    else {
        Simulation::start();
    }
}
void drawSimulationPauseGUI() {
    std::string buttonText = Simulation::on() ? "Disable" : "Enabled";
    ImGui::UpdateCurrentFontSize(modeButtonFontSize);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textButtonPadding);
    ImGui::Text("%s", std::format("Simulation: {}", Simulation::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - textButtonPadding);
    StyleVarHolder holder(ImGuiStyleVar_ButtonTextAlign, buttonAlignImVec);
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - modeToggleButtonSize.x
    );
    if (ImGui::Button(std::format("{}##simulation_button", buttonText).c_str(), modeToggleButtonSize)) {
        toggleSimulation();
    }
}
void drawElectricToggleGUI() {
    std::string buttonText = Electric::on() ? "Disable" : "Enable";
    ImGui::UpdateCurrentFontSize(modeButtonFontSize);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textButtonPadding);
    ImGui::Text("%s", std::format("Electric: {}", Electric::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - textButtonPadding);
    StyleVarHolder holder(ImGuiStyleVar_ButtonTextAlign, buttonAlignImVec);
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - modeToggleButtonSize.x
    );
    if (ImGui::Button(std::format("{}##electric_button", buttonText).c_str(), modeToggleButtonSize)) {
        toggleElectric();
    }
}
void drawGravityToggleGUI() {
    std::string buttonText = Gravity::on() ? "Disable" : "Enable";
    ImGui::UpdateCurrentFontSize(modeButtonFontSize);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textButtonPadding);
    ImGui::Text("%s", std::format("Gravity: {}", Gravity::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - textButtonPadding);
    StyleVarHolder holder(ImGuiStyleVar_ButtonTextAlign, buttonAlignImVec);
    ImGui::SetCursorPosX(
        ImGui::GetCursorPosX()
        + ImGui::GetContentRegionAvail().x
        - modeToggleButtonSize.x
    );
    if (ImGui::Button(std::format("{}##gravity_button", buttonText).c_str(), modeToggleButtonSize)) {
        toggleGravity();
    }
}
void drawModeToggleGUI() {
    ImGui::BeginChild("ModeToggleGUI", modeToggleSize, ImGuiChildFlags_Borders);
    ImGui::UpdateCurrentFontSize(modeButtonFontSize);
    ImGui::Text("Toggle Modes");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + modeToggleHeaderPadding);
    ImGui::BeginGroup();
    drawSimulationPauseGUI();
    drawElectricToggleGUI();
    drawGravityToggleGUI();
    ImGui::EndGroup();
    ImGui::EndChild();
}
int newParticleX = 0;
int newParticleY = 0;
int newParticleVelX = 0;
int newParticleVelY = 0;
float newParticleRadius = 0;
void drawParticleCreationGUI() {
    ImGui::BeginChild("ParticleCreationGUI", particleCreationSize, ImGuiChildFlags_Borders);
    ImGui::Text("New Particle");
    ImGui::BeginGroup();
    ImGui::Text("Size");
    ImGui::InputFloat("Radius", &newParticleRadius);
    ImGui::Text("Position");
    ImGui::InputInt("X##new_particle_position_x", &newParticleX);
    ImGui::InputInt("Y##Xnew_particle_position_y", &newParticleY);
    ImGui::Text("Initial Velocity");
    ImGui::InputInt("X##new_particle_velocity_x", &newParticleVelX);
    ImGui::InputInt("Y##new_particle_velocity_y", &newParticleVelY);
    ImGui::Text("Create Particle");
    if (ImGui::Button("Create", particleCreationButtonSize)) {
        Particles::add(
            Position{static_cast<float>(newParticleX), static_cast<float>(newParticleY)},
            Velocity{static_cast<float>(newParticleVelX), static_cast<float>(newParticleVelY)},
            newParticleRadius
        );
    }
    ImGui::EndGroup();
    ImGui::EndChild();
}
void drawAboutWindow() {
    ImGui::Text("PSim Ultimate");
    ImGui::TextColored(appVersionColorRGBA,
        "%s", std::format("Version {}", PSimImpl::PSimAppVersion_).c_str());
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("© 2026 etaiami09-cmd. MIT License.");
    ImGui::Spacing();

    ImGui::Text("Project Homepage:");
    ImGui::SameLine();
    
    if (ImGui::TextLink("Visit GitHub")) {
        openShellURL(repoURL);
    }
}
void drawGUI() {
    rlImGuiBegin();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(GUIWidth, GUIHeight));
    ImGui::Begin("Controls", nullptr,
        ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
    );
    
    drawModeToggleGUI();
    drawParticleCreationGUI();
    drawAboutWindow();
    ImGui::End();
    
    rlImGuiEnd();
}

void drawElectricParticles() {
    auto particles = Particles::get();
    for (size_t i = 0; i < particles.size(); i++) {
        Color color = GRAY;
        if (auto charge = Electric::get(i);
            charge.has_value() && *charge != 0) {
            color = (charge.value() > 0) ? BLUE : RED;
        }
        DrawCircle(
            static_cast<int>(particles[i].getPosition().x),
            static_cast<int>(particles[i].getPosition().y),
            particles[i].getRadius(), color
        );
    }
}

void drawParticles() {
    if (Electric::on()) {
        drawElectricParticles();
    }
    else {
        auto particles = Particles::get();
        for (const auto& particle : particles) {
            DrawCircle(
                static_cast<int>(particle.getPosition().x),
                static_cast<int>(particle.getPosition().y),
                particle.getRadius(), RED);
        }
    }
}
} // namespace

void drawFrame() {
    // placeholder
    BeginDrawing();
    ClearBackground(WHITE);
    drawParticles();
    drawGUI();
    EndDrawing();
}