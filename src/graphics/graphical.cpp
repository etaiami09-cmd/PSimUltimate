#include "graphical.hpp"

#include "graphical-impl.hpp"
#include "menu_bar.hpp"
#include "graphics_callbacks.hpp"
#include "imgui_internal.h"
#include "module_gui.hpp"
#include "raylib.h"
#include "module.hpp"
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
    ImGuiStyle& style = ImGui::GetStyle();
    const ImVec4& headerColor = style.Colors[ImGuiCol_TitleBgActive];
    style.Colors[ImGuiCol_TitleBg] = headerColor;
    style.Colors[ImGuiCol_TitleBgCollapsed] = headerColor;
}

void closeWindow() {
    rlImGuiShutdown();
    CloseWindow();
}

int getPSimGUIWidth() {
    return PSimImpl::GUIWidth;
}

namespace {

void drawGUI() {
    rlImGuiBegin();
    drawMenuBar();
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2{GUIWidth, static_cast<float>(GetScreenHeight()) - ImGui::GetFrameHeight()});
    ImGui::Begin("Controls", nullptr,
        ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
    );
    
    drawModeToggleGUI();
    ImGui::UpdateCurrentFontSize(ImGui::GetDefaultFont()->LegacySize);
    drawParticleCreationGUI();
    drawConstantsGUI();
    drawModuleGUI();
    drawAboutWindow();
    ImGui::End();
    rlImGuiEnd();
}

void drawParticles() {
    auto particles = Particles::get();
    for (const auto& particle : particles) {
        DrawCircle(
static_cast<int>(particle.getPosition().x + GUIWidth),
static_cast<int>(particle.getPosition().y),
particle.getRadius(), RED);
    }
}
} // namespace

void drawFrame() {
    // placeholder
    BeginDrawing();
    ClearBackground(WHITE);
    drawParticles();
    for (const auto& graphicsHandler : getGraphicsHandlers()) {
        if (getModules()[graphicsHandler.moduleIndex].active) {
            graphicsHandler(Particles::get());
        }
    }
    drawGUI();
    EndDrawing();
}