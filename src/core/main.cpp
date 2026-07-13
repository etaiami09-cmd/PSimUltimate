#define PSIM_MAIN_CPP

#include "app_data_utils.hpp"
#include "raylib.h"
#include "graphical.hpp"
#include "simulation.hpp"
#include "particles.hpp"
#include "controls.hpp"
#include "builtin_modules.hpp"
#include "configs.hpp"
#include "fps.hpp"
#include "load_module.hpp"

int main() {
    createAppDataFolderIfNotExists();
    loadTicksPerFrameConfig();
    registerKeybinds();
    loadBuiltinModuleTable();
    initElectrostaticModule();
    initGravityModule();
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
