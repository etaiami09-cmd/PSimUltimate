#include <algorithm>
#include <vector>
#include <string>
#include <span>
#include <optional>

#include "module.hpp"

namespace {

std::vector<Module> modules;

std::optional<Module&> getModuleByName(const std::string& name) noexcept {
    auto search = std::ranges::find_if(modules,
        [&](auto& module) {
            return module.name == name;
        }
    );
    if (search == modules.end()) {
        return {};
    }

    return std::optional<Module&>{*search};
}

} // namespace

void addModule(const std::string& name) noexcept {
    if (!getModuleByName(name).has_value()) {
        modules.emplace_back(name, false);
    }
}

std::span<Module> getModules() {
    return modules;
}