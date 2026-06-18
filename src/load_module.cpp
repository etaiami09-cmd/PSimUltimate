#include <string>

#include "load_module.hpp"
#include "psim_module_api.hpp"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <libloaderapi.h>
#else
#include <dlfcn.h>
#endif

namespace {
PSIM_Module_Function_Table functionTable{regModule,
    regConstant, regAttribute, regForce,
    regVelocity, regPosition, regRenderer};
} // namespace

void loadBuiltinModuleTable() noexcept {
    loadModuleSystem(&functionTable);
}

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
void loadModule(const std::string& dllName) {
#if defined(_WIN32)
    auto* module = LoadLibraryA(dllName.c_str());
    auto entry = GetProcAddress(module, "PSIM_Initialize_Module");
    auto callable = reinterpret_cast<void(PSIM_CALL*)(const PSIM_Module_Function_Table*)>(entry);
    callable(&functionTable);
#else
    auto* handle = dlopen(dllName, RTLD_NOW | RTLD_LOCAL);
    dlerror();
    auto entry = reinterpret_cast<void(*)(const PSIM_Module_Function_Table*)>(
        dlsym(module, "PSIM_Initialize_Module")
    );
    entry(&functionTable);
#endif
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)