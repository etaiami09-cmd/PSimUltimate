#include <algorithm>
#include <vector>
#include <string>
#include <span>
#include <optional>
#include <list>
#include <ranges>

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

size_t getRawModuleIndexByName(const std::string& name) {
    auto search = std::ranges::find_if(modules, [&](auto &module) {
        return module.name == name;
    });
    return search - modules.begin();
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

bool moduleExists(const std::string& name) noexcept {
    return std::ranges::any_of(modules, [&](const auto& module) {
        return module.name == name;
    });
}

void setModuleOrders(const std::span<std::string>& moduleOrders) noexcept {
    std::list<size_t> newOrder{};
    std::vector<size_t> indices = std::views::iota(0uz, modules.size())
                                    | std::ranges::to<std::vector<size_t>>();
    for (const auto& module : moduleOrders) {
        size_t rawModuleIndex = getRawModuleIndexByName(module);
        newOrder.push_back(rawModuleIndex);
        std::erase(indices, rawModuleIndex);
    }
    for (auto index : indices) {
        newOrder.push_back(index);
    }
    moduleIndexOrder = std::move(newOrder);
}
