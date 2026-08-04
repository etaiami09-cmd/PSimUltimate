#ifndef PSIM_LOAD_MODULE_HPP
#define PSIM_LOAD_MODULE_HPP

#include <string>

#if defined(_WIN32)
using ModuleHandle = struct HINSTANCE__*;
#else
using ModuleHandle = void*;
#endif

void loadBuiltinModuleTable() noexcept;
void loadModule(const std::string& dllName);
void unloadModule(ModuleHandle handle);
ModuleHandle getCurrentModuleHandle() noexcept;
void loadSavedModules();

#endif