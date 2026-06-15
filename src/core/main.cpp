#include "raylib.h"
#include "graphical.hpp"
#include "simulation.hpp"
#include "particles.hpp"

int main() {
    startWindow();

    while (!WindowShouldClose()) {
        if (Simulation::on()) {
            Particles::tick();
        }
        drawFrame();
    }
    closeWindow();
    return 0;
}