#include <string>

#include "load_module.hpp"

#include "pop_up_alerts.hpp"
#include "psim_module_api.hpp"

#if defined(_WIN32)
// 1. Temporarily rename Raylib's clashing types to a dummy name
#define Rectangle Raylib_Rectangle_Internal
#define CloseWindow Raylib_CloseWindow_Internal
#define ShowCursor Raylib_ShowCursor_Internal
#define DrawText Raylib_DrawText_Internal

// 2. Include Windows safely
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <libloaderapi.h>
#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef DrawText
#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef DrawText
#undef min
#undef max
#else
#include <dlfcn.h>
#endif

namespace {
PSIM_Module_Function_Table functionTable{regModule,
    regConstant, regAttribute, regForce,
    regVelocity, regPosition, regRenderer,
    accConstant, accAttribute,
    regKeybind, regTopMenuButton, regAlert, wrConfig,
    rdConfig, regSwitch, regSerialize, regDeserialize, regNonToggleableModule};

ModuleHandle currentModuleHandle;

} // namespace

void loadBuiltinModuleTable() noexcept {
    loadModuleSystem(&functionTable);
}

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
void loadModule(const std::string& dllName) {
#if defined(_WIN32)
    ModuleHandle module = LoadLibraryA(dllName.c_str());
	auto entry = GetProcAddress(module, "PSIM_Initialize_Module");
    auto callable = reinterpret_cast<void(PSIM_CALL*)(const PSIM_Module_Function_Table*)>(entry);
    if (callable == nullptr) {
        pushPopUpAlert("Error: Could not find module entry point.");
        return;
    }
	currentModuleHandle = module;
    callable(&functionTable);
	currentModuleHandle = {};
#else
    ModuleHandle handle = dlopen(dllName, RTLD_NOW | RTLD_LOCAL);
    dlerror();
    auto entry = reinterpret_cast<void(*)(const PSIM_Module_Function_Table*)>(
        dlsym(module, "PSIM_Initialize_Module")
    );
    if (entry == nullptr) {
        pushPopUpAlert("Error: Could not find module entry point.");
        return;
    }
    currentModuleHandle = module;
	entry(&functionTable);
	currentModuleHandle = {};
#endif
}

void unloadModule(ModuleHandle handle) {
#if defined(_WIN32)
	FreeLibrary(handle);
#else
	dlclose(handle);
#endif
}

ModuleHandle getCurrentModuleHandle() noexcept {
	return currentModuleHandle;
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
