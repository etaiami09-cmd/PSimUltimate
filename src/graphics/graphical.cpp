#include <string>
#include <cstddef>
#include <format>

#include "graphical.hpp"

#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "particles.hpp"

namespace {
constexpr const char* windowTitle = "Particle Simulator Ultimate";
constexpr int windowWidth = 1270;
constexpr int windowHeight = 720;
constexpr int targetFPS = 60;
constexpr float defaultRadius = 15;
constexpr int fontSize = 25;
constexpr float textButtonPadding = 5.0;
constexpr ImVec2 buttonAlignImVec(0.5, 0.5);
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
constexpr int GUIWidth = 1270;
constexpr int GUIHeight = 120;
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
void drawElectricGUI() {
    std::string buttonText = Electric::on() ? "Disable" : "Enable";
    ImGui::UpdateCurrentFontSize(fontSize);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textButtonPadding);
    ImGui::Text("%s", std::format("Electric {}", Electric::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - textButtonPadding);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, buttonAlignImVec);
    if (ImGui::Button(std::format("{}##electric_button", buttonText).c_str(), ImVec2(140, 40))) {
        toggleElectric();
    }
    ImGui::PopStyleVar();
}
void drawGravityGUI() {
    std::string buttonText = Gravity::on() ? "Disable" : "Enable";
    ImGui::UpdateCurrentFontSize(fontSize);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textButtonPadding);
    ImGui::Text("%s", std::format("Gravity {}", Gravity::on() ? "On" : "Off").c_str());
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - textButtonPadding);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, buttonAlignImVec);
    if (ImGui::Button(std::format("{}##gravity_button", buttonText).c_str(), ImVec2(140, 40))) {
        toggleGravity();
    }
    ImGui::PopStyleVar();
}
void drawGUI() {
    rlImGuiBegin();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(GUIWidth, GUIHeight));
    ImGui::Begin("Controls", nullptr,
        ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse);
    drawElectricGUI();
    drawGravityGUI();
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
            defaultRadius, color
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
                defaultRadius, RED);
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