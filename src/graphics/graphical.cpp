#include <cstddef>

#include "graphical.hpp"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "particles.hpp"
#include "Particle.hpp"
#include "toggles_gui.hpp"
#include "particle_creation_gui.hpp"
#include "constants_gui.hpp"
#include "about_window.hpp"

namespace {
constexpr const char* windowTitle = "Particle Simulator Ultimate";
constexpr int windowWidth = 1270;
constexpr int windowHeight = 720;
constexpr int targetFPS = 60;
constexpr int GUIWidth = 300;
} // namespace

void startWindow() {
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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

void drawGUI() {
    rlImGuiBegin();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2{GUIWidth, static_cast<float>(GetScreenHeight())});
    ImGui::Begin("Controls", nullptr,
        ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
    );
    
    drawModeToggleGUI();
    ImGui::UpdateCurrentFontSize(ImGui::GetDefaultFont()->LegacySize);
    drawParticleCreationGUI();
    drawConstantsGUI();
    drawAboutWindow();
    ImGui::End();
    
    rlImGuiEnd();
}

void drawElectricParticles() {
    auto particles = Particles::get();
    for (size_t i = 0; i < particles.size(); i++) {
        auto color = GRAY;
        if (auto charge = Electric::get(i);
            charge.has_value() && *charge != 0) {
            color = (charge.value() > 0) ? BLUE : RED;
        }
        DrawCircle(
            static_cast<int>(particles[i].getPosition().x + GUIWidth),
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
                static_cast<int>(particle.getPosition().x + GUIWidth),
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