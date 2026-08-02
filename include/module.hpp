#ifndef PSIM_MODULE_HPP
#define PSIM_MODULE_HPP

#include <string>
#include <list>
#include <span>
#include <functional>
#include <optional>

#include "load_module.hpp"
#include "pop_up_alerts.hpp"

struct Module {
    std::string name;
	ModuleHandle handle;
    std::optional<bool> active;

    void toggle() {
        if (active.has_value()) {
	        active.value() = !active.value();
        }
    	else {
    		pushPopUpAlert("Error: Attempted to toggle non-toggleable module. Please submit a bug report.");
    	}
    }
};

template<typename... Args>
struct ModuleCallback {
    std::function<void(Args...)> callback;
    size_t moduleIndex;

    void operator()(Args... args) const {
        callback(args...);
    }
    
};

void addModule(const std::string& name, ModuleHandle handle, bool toggleable) noexcept;
std::span<Module> getModules();
const std::list<size_t>& getModuleIndexOrders();
void repositionModule(size_t from, size_t to) noexcept;
bool isModuleActive(const std::string& name) noexcept;
bool isModuleActive(const Module& module) noexcept;
bool moduleExists(const std::string& name) noexcept;
void setModuleOrders(const std::span<std::string>& moduleOrders) noexcept;
void removeModule(const std::string& name);

#endif