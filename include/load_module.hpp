#ifndef PSIM_LOAD_MODULE_HPP
#define PSIM_LOAD_MODULE_HPP

#include <string>

void loadBuiltinModuleTable() noexcept;
void loadModule(const std::string& dllName);

#endif