#include <memory>
#include <ranges>
#include <string>
#include <functional>
#include <span>
#include <utility>
#include <cstddef>
#include <algorithm>

#include "constants.hpp"
#include "Particle.hpp"
#include "physics_callbacks.hpp"
#include "module.hpp"
#include "attributes.hpp"
#include "graphics_callbacks.hpp"

#include "psim_module_api.hpp"

#include "controls.hpp"
#include "menu_bar.hpp"
#include "pop_up_alerts.hpp"

namespace {

std::function<void()>
    restructureFunction(PSIM_Void_Callback function) {
    auto handle = std::shared_ptr<void>(function.capture, function.destroy);
    return [function, handle = std::move(handle)]() {
        function.invoke(function.capture);
    };
}

std::function<void(std::span<Particle>, std::span<Force>)>
    restructureFunction(PSIM_Force_Callback function)
{
    auto handle = std::shared_ptr<void>(function.capture, function.destroy);
    return [function, handle = std::move(handle)](std::span<Particle> particles, std::span<Force> forces) {
        function.invoke(function.capture, particles.data(), forces.data(), particles.size());
    };
}

std::function<void(std::span<Particle>)>
    restructureFunction(PSIM_Callback function)
{
    auto handle = std::shared_ptr<void>(function.capture, function.destroy);
    return [function, handle = std::move(handle)](std::span<Particle> particles) {
        function.invoke(function.capture, particles.data(), particles.size());
    };
}

std::function<void(std::span<const Particle>)>
    restructureFunction(PSIM_Graphics_Callback function)
{
    auto handle = std::shared_ptr<void>(function.capture, function.destroy);
    return [function, handle = std::move(handle)](std::span<const Particle> particles) {
        function.invoke(function.capture, particles.data(), particles.size());
    };
}

std::function<void(float)>
    restructureFunction(PSIM_Constant_Change_Callback function)
{
    auto handle = std::shared_ptr<void>(function.capture, function.destroy);
    return [function, handle = std::move(handle)](float newValue) {
        function.invoke(function.capture, newValue);
    };
}

size_t getModuleIndexByName(const std::string& name) {
    return *std::ranges::find_if(std::ranges::views::iota(size_t{0}, getModules().size()),
        [&](const auto& index) {
            return getModules()[index].name == name;
    });
}

} // namespace

extern "C" void PSIM_CALL regModule(const char* module, size_t moduleLen) {
    addModule(std::string{module, moduleLen});
}

extern "C" void PSIM_CALL regConstant(const char* module, size_t moduleLen,
    const char* name, size_t len, float defaultValue,
    float minValue, float maxValue, PSIM_Constant_Change_Callback onChange)
{
    std::ignore = module;
    std::ignore = moduleLen;
    addConstant(std::string{name, len}, defaultValue,
        minValue, maxValue, restructureFunction(onChange));
}

extern "C" void PSIM_CALL regForce(const char* module, size_t moduleLen,
    PSIM_Force_Callback forceHandler) {
    addForceHandler(
        ForceHandler{
            restructureFunction(forceHandler),
            getModuleIndexByName(std::string{module, moduleLen}),
        }
    );
}

extern "C" void PSIM_CALL regVelocity(const char* module, size_t moduleLen,
    PSIM_Callback velocityHandler) {
    addVelocityHandler(
        VelocityHandler{
            restructureFunction(velocityHandler),
            getModuleIndexByName(std::string{module, moduleLen}),
        }
    );
}

extern "C" void PSIM_CALL regPosition(const char* module, size_t moduleLen,
    PSIM_Callback positionHandler) {
    addPositionHandler(
        PositionHandler{
            restructureFunction(positionHandler),
            getModuleIndexByName(std::string{module, moduleLen}),
        }
    );
}

extern "C" void PSIM_CALL regAttribute(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    float defaultValue, float minValue, float maxValue) {
    std::ignore = module;
    std::ignore = moduleLen;
    addAttribute(std::string{name, nameLen}, defaultValue, minValue, maxValue);
}

extern "C" void PSIM_CALL regRenderer(const char* module, size_t moduleLen, PSIM_Graphics_Callback renderer) {
    addGraphicsHandler(
        GraphicsHandler{
            restructureFunction(renderer),
            getModuleIndexByName(std::string{module, moduleLen}),
        }
    );
}

extern "C" void PSIM_CALL accConstant(const char* name, size_t nameLen, bool* hasValue, float* retAddress) {
    std::string constantName{name, nameLen};
    auto result = getConstantValue(constantName);
    if (result.has_value()) {
        *retAddress = result.value();
        *hasValue = true;
    }
    else {
        *hasValue = false;
    }
}

extern "C" void PSIM_CALL accAttribute(const char* name, size_t nameLen, bool* hasValue, float** buffAddress, size_t* countAddress) {
    std::string attributeName{name, nameLen};
    auto result = getAttributeByName(attributeName);
    if (result.has_value()) {
        auto attribute = result.value();
        *buffAddress = attribute.values.data();
        *countAddress = attribute.values.size();
        *hasValue = true;
    }
    else {
        *hasValue = false;
    }
}

extern "C" void PSIM_CALL regKeybind(const char* module, size_t moduleLen, const char* name, size_t nameLen,
    const KeyboardKey* keys, size_t keysLen, const MouseButton* buttons, size_t buttonsLen,
    PSIM_Void_Callback callback) {
    std::string moduleName{module, moduleLen};
    std::string keybindName{name, nameLen};
    std::span<const KeyboardKey> keybindKeys{keys, keysLen};
    std::span<const MouseButton> keybindButtons{buttons, buttonsLen};
    addKeybind(moduleName, keybindName, keybindKeys, keybindButtons, restructureFunction(callback));
}

extern "C" void PSIM_CALL regTopMenuButton(const char* module, size_t moduleLen, const char* name,
    size_t nameLen, PSIM_Void_Callback callback) {
    std::string buttonName{name, nameLen};
    std::string moduleName{module, moduleLen};
    addModuleTopMenuButton(moduleName, buttonName, restructureFunction(callback));
}

void regAlert(const char *module, size_t moduleLen, const char *text, size_t textLen) {
    std::ignore = module;
    std::ignore = moduleLen;
    pushPopUpAlert(std::string{text, textLen});
}
