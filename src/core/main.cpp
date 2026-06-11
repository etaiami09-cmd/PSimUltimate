#include "raylib.h"
#include "graphical.hpp"
#include "Particle.hpp"
#include "particles.hpp"
#include <iostream>

int main() {

    startWindow();

    while (!WindowShouldClose()) {
        Particles::tick(0.016f);
        drawFrame();
    }

    closeWindow();
    return 0;
}