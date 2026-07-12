#include "graphical.hpp"

#include "graphical-impl.hpp"
#include "menu_bar.hpp"
#include "graphics_callbacks.hpp"
#include "imgui_internal.h"
#include "module_gui.hpp"
#include "raylib.h"
#include "module.hpp"
#include "rlImGui.h"
// NOTE: The ImGui used for this project has been modified to remove bottom-right
// resize grips on windows and remove resize grips from every border except the right
// one of a window. It is not done cleanly through settings but by just commenting
// out the normal code. Changes are in imgui.cpp. TAKE NOTE!
#include "imgui.h"
#include "particles.hpp"
#include "Particle.hpp"
#include "toggles_gui.hpp"
#include "particle_creation_gui.hpp"
#include "constants_gui.hpp"
#include "about_window.hpp"
#include "pop_up_alerts.hpp"

namespace {
constexpr const char *windowTitle = "Particle Simulator Ultimate";
constexpr int windowWidth = 1270;
constexpr int windowHeight = 720;
constexpr int targetFPS = 60;
constexpr int minGUIWidth = 200;
constexpr int defaultGUIWidth = 320;
ImVec2 currentWindowSize;
bool firstFrame = true;
} // namespace

void startWindow() {
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(targetFPS);
    ImGui::CreateContext();
    ImGui::PushFont(ImGui::GetIO().Fonts->AddFontDefaultVector(), 13.0f);
    rlImGuiSetup(true);
    ImGuiStyle &style = ImGui::GetStyle();
    style.FontSizeBase = 18.0f;
    const ImVec4 &headerColor = style.Colors[ImGuiCol_TitleBgActive];
    style.Colors[ImGuiCol_TitleBg] = headerColor;
    style.Colors[ImGuiCol_TitleBgCollapsed] = headerColor;
}

void closeWindow() {
    rlImGuiShutdown();
    CloseWindow();
}

float getPSimGUIWidth() {
    return currentWindowSize.x;
}

namespace {
void drawGUI() {
    rlImGuiBegin();
    drawMenuBar();
    if (firstFrame) {
        ImGui::SetNextWindowSize(ImVec2{
            defaultGUIWidth,
            static_cast<float>(GetScreenHeight()) - ImGui::GetFrameHeight()});
    }
    else {
        ImGui::SetNextWindowSizeConstraints(
        ImVec2{
                static_cast<float>(minGUIWidth),
                static_cast<float>(GetScreenHeight()) - ImGui::GetFrameHeight()
            },
            ImVec2{
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight()) - ImGui::GetFrameHeight()
            }
        );
    }
    firstFrame = false;
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::Begin("Controls", nullptr,
                 ImGuiWindowFlags_NoMove
                 | ImGuiWindowFlags_NoCollapse
    );
    currentWindowSize = ImGui::GetWindowSize();

    drawModeToggleGUI();
    drawParticleCreationGUI();
    drawConstantsGUI();
    drawModuleGUI();
    drawAboutWindow();
    ImGui::End();
    displayPopups();
    rlImGuiEnd();
}

void drawParticles() {
    auto particles = Particles::get();
    for (const auto &particle: particles) {
        DrawCircle(
            static_cast<int>(particle.getPosition().x + getPSimGUIWidth()),
            static_cast<int>(particle.getPosition().y + ImGui::GetFrameHeight()),
            particle.getRadius(), RED);
    }
}
} // namespace

void drawFrame() {
    BeginDrawing();
    ClearBackground(WHITE);
    drawParticles();
    for (const auto moduleIndex: getModuleIndexOrders()) {
        const auto &module = getModules()[moduleIndex];
        if (module.active) {
            for (const auto &graphicsHandler: getGraphicsHandlers()) {
                if (graphicsHandler.moduleIndex == moduleIndex) {
                    graphicsHandler(Particles::get());
                }
            }
        }
    }
    drawGUI();
    EndDrawing();
}
