#include <memory>
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

namespace {

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

Module& getModuleByName(const std::string& name) {
    return *std::ranges::find_if(getModules(), [&](const auto& moduleName) {
        return name == moduleName;
        }, &Module::name
    );
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
            getModuleByName(std::string{module, moduleLen}),
        }
    );
}

extern "C" void PSIM_CALL regVelocity(const char* module, size_t moduleLen,
    PSIM_Callback velocityHandler) {
    addVelocityHandler(
        VelocityHandler{
            restructureFunction(velocityHandler),
            getModuleByName(std::string{module, moduleLen}),
        }
    );
}

extern "C" void PSIM_CALL regPosition(const char* module, size_t moduleLen,
    PSIM_Callback positionHandler) {
    addPositionHandler(
        PositionHandler{
            restructureFunction(positionHandler),
            getModuleByName(std::string{module, moduleLen}),
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
            getModuleByName(std::string{module, moduleLen}),
        }
    );
}