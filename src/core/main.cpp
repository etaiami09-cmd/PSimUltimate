#define PSIM_MAIN_CPP

#include "app_data_utils.hpp"
#include "graphical.hpp"
#include "simulation.hpp"
#include "particles.hpp"
#include "controls.hpp"
#include "builtin_modules.hpp"
#include "load_module.hpp"
#include "shutdown.hpp"
#include "shutdown_hooks.hpp"

int main() {
    createAppDataFolderIfNotExists();
    loadTicksPerFrameConfig();
    registerKeybinds();
    loadBuiltinModuleTable();
    initElectrostaticModule();
    initGravityModule();
	loadSavedModules();
    startWindow();

    while (!shouldShutdown()) {
        if (Simulation::on()) {
            Particles::tick();
        }
        handleControls();
        drawFrame();
    }

    closeWindow();
	runShutdownHooks();
    return 0;
}
