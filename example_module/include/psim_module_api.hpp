#ifndef PSIM_MODULE_API_HPP
#define PSIM_MODULE_API_HPP

#if defined(_WIN32)
#define PSIM_CALL __cdecl
#else
#define PSIM_CALL
#endif

#if defined(_WIN32)
#  define PSIM_MODULE_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#  define PSIM_MODULE_EXPORT __attribute__((visibility("default")))
#else
#  define PSIM_MODULE_EXPORT
#endif

#ifndef PSIM_STATIC_BUILD
#define RAYLIB_SHARED
#endif

#include <optional>
#include <span>
#include <string>
#include <functional>
#include <cstddef>
#include <utility>
#include <type_traits>

#include "Particle.hpp"

#include "raylib.h"

constexpr size_t particleSize = 6 * sizeof(float);
constexpr size_t forceSize = 2 * sizeof(float);

static_assert(std::is_standard_layout_v<Particle> && std::is_trivially_copyable_v<Particle>);
static_assert(sizeof(Particle) == particleSize);
static_assert(std::is_standard_layout_v<Force> && std::is_trivially_copyable_v<Force>);
static_assert(sizeof(Force) == forceSize);

// Register a new module
inline void registerModule(const std::string& name);
// Register a new constant
inline void registerConstant(const std::string& module, const std::string& name, float defaultValue,
    float minValue, float maxValue, const std::function<void(float)>& onChange);
// Register a new particle attribute
inline void registerParticleAttribute(const std::string& module, const std::string& name, float defaultValue,
    float minValue, float maxValue);
// Register a function that changes the force on a particle
inline void registerForce(const std::string& module,
    const std::function<void(std::span<Particle>, std::span<Force>)>& forceHandler);
// Register a function that changes the velocity of a particle
inline void registerVelocityHandler(const std::string& module,
    const std::function<void(std::span<Particle>)>& velocityHandler);
// Register a function that changes the position of a particle
inline void registerPositionHandler(const std::string& module,
    const std::function<void(std::span<Particle>)>& positionHandler);
// Register a function that runs every time a frame is drawn
inline void registerRenderer(const std::string& module,
    const std::function<void(std::span<const Particle>)>& renderer);

void initializePSimModule();

extern "C" {

struct PSIM_Callback {
    void (PSIM_CALL *invoke)(void* capture, Particle* particles, size_t count);
    void (PSIM_CALL *destroy)(void* capture);
    void* capture;
};

struct PSIM_Void_Callback {
    void (PSIM_CALL *invoke)(void* capture);
    void (PSIM_CALL *destroy)(void* capture);
    void* capture;
};

struct PSIM_Graphics_Callback {
    void (PSIM_CALL *invoke)(void* capture, const Particle* particles, size_t count);
    void (PSIM_CALL *destroy)(void* capture);
    void* capture;
};

struct PSIM_Force_Callback {
    void (PSIM_CALL *invoke)(void* capture, Particle* particles, Force* forces, size_t count);
    void (PSIM_CALL *destroy)(void* capture);
    void* capture;
};

struct PSIM_Constant_Change_Callback {
    void (PSIM_CALL *invoke)(void* capture, float newValue);
    void (PSIM_CALL *destroy)(void* capture);
    void* capture;
};

struct PSIM_Boolean_Change_Callback {
    void (PSIM_CALL *invoke)(void* capture, bool newValue);
    void (PSIM_CALL *destroy)(void* capture);
    void* capture;
};

void PSIM_CALL regModule(const char* module, size_t moduleLen);
void PSIM_CALL regConstant(const char* module, size_t moduleLen, const char* name, size_t nameLen, float defaultValue,
    float minValue, float maxValue, PSIM_Constant_Change_Callback onChange);
void PSIM_CALL regAttribute(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    float defaultValue, float minValue, float maxValue);
void PSIM_CALL regForce(const char* module, size_t moduleLen, PSIM_Force_Callback forceHandler);
void PSIM_CALL regVelocity(const char* module, size_t moduleLen, PSIM_Callback velocityHandler);
void PSIM_CALL regPosition(const char* module, size_t moduleLen, PSIM_Callback positionHandler);
void PSIM_CALL regRenderer(const char* module, size_t moduleLen, PSIM_Graphics_Callback renderer);
void PSIM_CALL accConstant(const char* name, size_t nameLen, bool* hasValue, float* retAddress);
void PSIM_CALL accAttribute(const char* name, size_t nameLen, bool* hasValue, float** buffAddress, size_t* countAddress);
void PSIM_CALL regKeybind(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    const KeyboardKey* keys, size_t keysLen, const MouseButton* buttons, size_t buttonsLen,
    PSIM_Void_Callback callback);
void PSIM_CALL regTopMenuButton(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    PSIM_Void_Callback callback);
void PSIM_CALL regAlert(const char* module, size_t moduleLen, const char* text, size_t textLen);
void PSIM_CALL wrConfig(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    const void* data, size_t dataLen);
bool PSIM_CALL rdConfig(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    void* data, size_t dataLen);
void PSIM_CALL regSwitch(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    bool defaultValue, PSIM_Boolean_Change_Callback onChange);

struct PSIM_Module_Function_Table {
    decltype(regModule)* _regModule;
    decltype(regConstant)* _regConstant;
    decltype(regAttribute)* _regAttribute;
    decltype(regForce)* _regForce;
    decltype(regVelocity)* _regVelocity;
    decltype(regPosition)* _regPosition;
    decltype(regRenderer)* _regRenderer;
    decltype(accConstant)* _accConstant;
    decltype(accAttribute)* _accAttribute;
    decltype(regKeybind)* _regKeybind;
    decltype(regTopMenuButton)* _regTopMenuButton;
    decltype(regAlert)* _regAlert;
    decltype(wrConfig)* _wrConfig;
    decltype(rdConfig)* _rdConfig;
    decltype(regSwitch)* _regSwitch;
};

inline const PSIM_Module_Function_Table* PSIM_Api_Table;

#ifndef PSIM_STATIC_BUILD
inline PSIM_MODULE_EXPORT void PSIM_CALL PSIM_Initialize_Module(
    const PSIM_Module_Function_Table* functionTable)
{
    PSIM_Api_Table = functionTable;
    initializePSimModule();
}
#endif

}

inline void loadModuleSystem(const PSIM_Module_Function_Table* functionTable) {
    PSIM_Api_Table = functionTable;
}

inline PSIM_Void_Callback destructureFunction(std::function<void()> function) {
    using Fn = decltype(function);
    auto* held = new Fn(std::move(function));
    auto invoke = [](void* capture) noexcept {
        auto* f = static_cast<Fn*>(capture);
        try {
            (*f)();
        }
        catch (...) {}
    };
    auto destroy = [](void* capture) noexcept {
        delete static_cast<Fn*>(capture);
    };
    return PSIM_Void_Callback{invoke, destroy, held};
}

inline PSIM_Force_Callback destructureFunction(std::function<void(std::span<Particle>, std::span<Force>)> function) {
    using Fn = decltype(function);
    auto* held = new Fn(std::move(function));
    auto invoke = [](void* capture, Particle* particles, Force* forces, size_t count) noexcept {
        auto* f = static_cast<Fn*>(capture);
        try {
            (*f)(std::span<Particle>(particles, count), std::span<Force>(forces, count));
        }
        catch (...) {}
    };
    auto destroy = [](void* capture) noexcept {
        delete static_cast<Fn*>(capture);
    };
    return PSIM_Force_Callback{invoke, destroy, held};
}

inline PSIM_Callback destructureFunction(std::function<void(std::span<Particle>)> function) {
    using Fn = decltype(function);
    auto* held = new Fn(std::move(function));
    auto invoke = [](void* capture, Particle* particles, size_t count) noexcept {
        auto* f = static_cast<Fn*>(capture);
        try {
            (*f)(std::span<Particle>(particles, count));
        }
        catch (...) {}
    };
    auto destroy = [](void* capture) noexcept {
        delete static_cast<Fn*>(capture);
    };
    return PSIM_Callback{invoke, destroy, held};
}

inline PSIM_Constant_Change_Callback destructureFunction(std::function<void(float)> function) {
    using Fn = decltype(function);
    auto* held = new Fn(std::move(function));
    auto invoke = [](void* capture, float newValue) noexcept {
        auto* f = static_cast<Fn*>(capture);
        try {
            (*f)(newValue);
        }
        catch (...) {}
    };
    auto destroy = [](void* capture) noexcept {
        delete static_cast<Fn*>(capture);
    };
    return PSIM_Constant_Change_Callback{invoke, destroy, held};
}

inline PSIM_Boolean_Change_Callback destructureFunction(std::function<void(bool)> function) {
    using Fn = decltype(function);
    auto* held = new Fn(std::move(function));
    auto invoke = [](void* capture, bool newValue) noexcept {
        auto* f = static_cast<Fn*>(capture);
        try {
            (*f)(newValue);
        }
        catch (...) {}
    };
    auto destroy = [](void* capture) noexcept {
        delete static_cast<Fn*>(capture);
    };
    return PSIM_Boolean_Change_Callback{invoke, destroy, held};
}

inline PSIM_Graphics_Callback destructureFunction(std::function<void(std::span<const Particle>)> function) {
    using Fn = decltype(function);
    auto* held = new Fn(std::move(function));
    auto invoke = [](void* capture, const Particle* particles, size_t count) noexcept {
        auto* f = static_cast<Fn*>(capture);
        try {
            (*f)(std::span<const Particle>{particles, count});
        }
        catch (...) {}
    };
    auto destroy = [](void* capture) noexcept {
        delete static_cast<Fn*>(capture);
    };
    return PSIM_Graphics_Callback{invoke, destroy, held};
}

inline void registerConstant(const std::string& module, const std::string& name, float defaultValue,
    float minValue, float maxValue, const std::function<void(float)>& onChange)
{
    PSIM_Api_Table->_regConstant(module.c_str(), module.size(), name.c_str(),
    name.size(), defaultValue, minValue, maxValue, destructureFunction((onChange)));
}

inline void registerParticleAttribute(const std::string& module, const std::string& name, float defaultValue,
    float minValue, float maxValue) {
        PSIM_Api_Table->_regAttribute(module.c_str(), module.size(), name.c_str(), name.size(),
            defaultValue, minValue, maxValue);
}

inline void registerForce(const std::string& module,
    const std::function<void(std::span<Particle>, std::span<Force>)>& forceHandler) {
    PSIM_Api_Table->_regForce(module.c_str(), module.size(),
    destructureFunction(forceHandler));
}

inline void registerVelocityHandler(const std::string& module,
    const std::function<void(std::span<Particle>)>& velocityHandler) {
    PSIM_Api_Table->_regVelocity(module.c_str(), module.size(),
    destructureFunction(velocityHandler));
}

inline void registerPositionHandler(const std::string& module,
    const std::function<void(std::span<Particle>)>& positionHandler) {
    PSIM_Api_Table->_regPosition(module.c_str(), module.size(),
    destructureFunction(positionHandler));
}

inline void registerRenderer(const std::string& module,
    const std::function<void(std::span<const Particle>)>& renderer) {
    PSIM_Api_Table->_regRenderer(module.c_str(), module.size(),
    destructureFunction(renderer));
}

inline void registerModule(const std::string& name) {
    PSIM_Api_Table->_regModule(name.c_str(), name.size());
}

inline std::optional<float> accessConstantValue(const std::string& name) {
    float value;
    bool hasValue;
    PSIM_Api_Table->_accConstant(name.c_str(), name.size(), &hasValue, &value);
    if (!hasValue) {
        return {};
    }
    return value;
}

inline std::optional<std::span<float>> accessAttribute(const std::string& name) {
    float* values;
    size_t count;
    bool hasValue;
    PSIM_Api_Table->_accAttribute(name.c_str(), name.size(), &hasValue, &values, &count);
    if (!hasValue) {
        return {};
    }
    return std::span<float>(values, count);
}

inline void registerKeybind(const std::string& module, const std::string& name,
    std::span<KeyboardKey> keys, std::span<MouseButton> buttons,
    const std::function<void()>& callback) {
    PSIM_Api_Table->_regKeybind(module.c_str(), module.size(), name.c_str(), name.size(),
        keys.data(), keys.size(), buttons.data(), buttons.size(),
        destructureFunction(callback));
}

inline void registerTopMenuButton(const std::string& module, const std::string& name,
    const std::function<void()>& callback) {
    PSIM_Api_Table->_regTopMenuButton(module.c_str(), module.size(), name.c_str(), name.size(),
        destructureFunction(callback));
}

inline void alertUser(const std::string& module, const std::string& text) {
    PSIM_Api_Table->_regAlert(module.c_str(), module.size(),
        text.c_str(), text.size());
}

template<typename T>
void writeConfig(const std::string& module, const std::string& name, const T& value) {
    PSIM_Api_Table->_wrConfig(module.c_str(), module.size(), name.c_str(), name.size(),
        &value, sizeof(T));
}

template<typename T>
std::optional<T> readConfig(const std::string& module, const std::string& name) {
    T value;
    if (!PSIM_Api_Table->_rdConfig(module.c_str(), module.size(), name.c_str(), name.size(),
        &value, sizeof(T))) {
        return {};
    }
    return value;
}

inline void registerSwitch(const std::string& module, const std::string& name,
    bool defaultValue, const std::function<void(bool)>& onChange) {
    PSIM_Api_Table->_regSwitch(module.c_str(), module.size(), name.c_str(), name.size(),
        defaultValue, destructureFunction(onChange));
}

#endif