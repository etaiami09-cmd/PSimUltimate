#include "raylib.h"
#include "graphical.hpp"
#include "Particle.hpp"
#include "particles.hpp"
#include <iostream>

int main() {

    Particles::add(Position{1, 200}, Velocity{3, 4});
    Particles::add(Position{300, 400}, Velocity{30, 20});
    Electric::init();
    Electric::set(0, 1);
    Electric::set(1, 0);

    startWindow();

    while (!WindowShouldClose()) {
        // Particles::tickGravity();
        // Particles::tickPositions();
        drawFrame();
    }

    closeWindow();
    return 0;
}