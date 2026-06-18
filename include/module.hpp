#ifndef PSIM_MODULE_HPP
#define PSIM_MODULE_HPP

#include <string>
#include <span>
#include <functional>
#include <cstddef>

struct Module {
    std::string name;
    bool active;

    void toggle() {
        active = !active;
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

void addModule(const std::string& name) noexcept;
std::span<Module> getModules();

#endif