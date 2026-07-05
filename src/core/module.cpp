#include <algorithm>
#include <vector>
#include <string>
#include <span>
#include <optional>
#include <list>

#include "module.hpp"

namespace {
std::vector<Module> modules;

std::list<size_t> moduleIndexOrder;

std::optional<Module &> getModuleByName(const std::string &name) noexcept {
    auto search = std::ranges::find_if(modules,
                                       [&](auto &module) {
                                           return module.name == name;
                                       }
    );
    if (search == modules.end()) {
        return {};
    }

    return std::optional<Module &>{*search};
}
} // namespace

void addModule(const std::string &name) noexcept {
    if (!getModuleByName(name).has_value()) {
        modules.emplace_back(name, false);
        moduleIndexOrder.push_back(modules.size() - 1);
    }
}

std::span<Module> getModules() {
    return modules;
}

const std::list<size_t> &getModuleIndexOrders() {
    return moduleIndexOrder;
}

void repositionModule(size_t from, size_t to) noexcept {
    auto fromIt = std::next(moduleIndexOrder.begin(), from);
    if (from < to) {
        moduleIndexOrder.splice(std::next(moduleIndexOrder.begin(), to + 1),
            moduleIndexOrder, fromIt);
    }
    else {
        moduleIndexOrder.splice(std::next(moduleIndexOrder.begin(), to),
            moduleIndexOrder, fromIt);
    }
}

bool isModuleActive(const std::string &name) noexcept {
    auto search = std::ranges::find_if(modules, [&](auto &module) {
        return module.name == name;
    });
    if (search == modules.end()) {
        return false;
    }
    auto& module = *search;
    return module.active;
}
