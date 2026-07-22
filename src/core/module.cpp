#include <algorithm>
#include <format>
#include <vector>
#include <string>
#include <span>
#include <optional>
#include <list>
#include <ranges>

#include "attributes.hpp"
#include "constants.hpp"
#include "controls.hpp"
#include "graphics_callbacks.hpp"
#include "menu_bar.hpp"
#include "physics_callbacks.hpp"
#include "pop_up_alerts.hpp"
#include "serializer.hpp"
#include "settings_menu.hpp"

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

std::optional<size_t> getRawModuleIndexByName(const std::string& name) {
    auto search = std::ranges::find_if(modules, [&](auto &module) {
        return module.name == name;
    });
	if (search == modules.end()) {
		return {};
	}
	return search - modules.begin();
}

void removeModuleRegistries(size_t moduleIndex) noexcept {
	const auto& name = modules[moduleIndex].name;
	removeModuleConstants(name);
	removeModuleAttributes(name);
	removeModuleKeybinds(name);
	removeModuleTopMenuButtons(name);
	removeModuleSwitches(name);
	removeModuleSerializersAndDeserializers(name);
	cleanModulePhysicsCallbacks(moduleIndex);
	cleanModuleGraphicsCallbacks(moduleIndex);
}

} // namespace


void addModule(const std::string &name, ModuleHandle handle) noexcept {
    if (!getModuleByName(name).has_value()) {
        modules.emplace_back(name, handle, false);
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
        size_t rawModuleIndex = getRawModuleIndexByName(module).value();
        newOrder.push_back(rawModuleIndex);
        std::erase(indices, rawModuleIndex);
    }
    for (auto index : indices) {
        newOrder.push_back(index);
    }
    moduleIndexOrder = std::move(newOrder);
}

void removeModule(const std::string& name) {
	auto moduleIndex = getRawModuleIndexByName(name);
	if (!moduleIndex.has_value()) {
		pushPopUpAlert(std::format("Attempted to remove non-existing module '{}'.", name).c_str());
		return;
	}
	ModuleHandle handle = modules[moduleIndex.value()].handle;
	removeModuleRegistries(moduleIndex.value());
	modules.erase(modules.begin() + moduleIndex.value());
	moduleIndexOrder.remove(moduleIndex.value());
	if (!std::ranges::contains(modules, handle, &Module::handle)) {
		unloadModule(handle);
	}
	for (auto& index : moduleIndexOrder) {
		if (index > moduleIndex.value()) {
			index--;
		}
	}
}
