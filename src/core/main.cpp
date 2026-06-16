#include "raylib.h"
#include "graphical.hpp"
#include "simulation.hpp"
#include "particles.hpp"
#include "controls.hpp"

int main() {
    registerKeybinds();
    startWindow();

    while (!WindowShouldClose()) {
        if (Simulation::on()) {
            Particles::tick();
        }
        handleControls();
        drawFrame();
    }

    closeWindow();
    return 0;
}