#include <vector>
#include <string>
#include <span>

#include "module.hpp"

namespace {

std::vector<Module> modules;

} // namespace

void addModule(const std::string& name) noexcept {
    modules.emplace_back(name, false);
}

std::span<Module> getModules() {
    return modules;
}