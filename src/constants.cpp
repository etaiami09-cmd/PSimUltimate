#include <string>
#include <unordered_map>
#include <optional>
#include <functional>
#include <utility>

#include "constants.hpp"

namespace {
std::unordered_map<std::string, Constant> constants;
} // namespace

void Constant::set() {
    value = buffer;
    onChange(value);
}

void addConstant(const std::string& module, const std::string& name, float defaultValue,
                 float minValue, float maxValue, std::function<void(float)>&& onChange) noexcept {
    constants[name] = {module, defaultValue, defaultValue,
        defaultValue, minValue, maxValue, std::move(onChange)};
}

void resetConstant(const std::string& name) noexcept {
    constants[name].value = constants[name].defaultValue;
    constants[name].buffer = constants[name].defaultValue;
}

std::optional<float> getConstantValue(const std::string& name) noexcept {
    if (constants.contains(name)) {
        return constants[name].value;
    }
    return {};
}

std::unordered_map<std::string, Constant>& getAllConstants() noexcept {
    return constants;
}

std::optional<float*> getConstantBuffer(const std::string& name) noexcept {
    if (constants.contains(name)) {
        return &constants[name].buffer;
    }
    return {};
}

void removeModuleConstants(const std::string& module) noexcept {
	std::erase_if(constants, [&module](const auto& pair) {
		return module == pair.second.module;
	});
}
