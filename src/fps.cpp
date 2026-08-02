#include "raylib.h"

#include "fps.hpp"

#include "configs.hpp"

namespace {
int fps = 60;
} // namespace

int getFPS() {
    return fps;
}

void setFPS(int newFPS) {
    SetTargetFPS(newFPS);
    fps = newFPS;
    setConfigValue("FPS", newFPS);
}