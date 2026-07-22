#ifndef PSIM_CONSTANTS_HPP
#define PSIM_CONSTANTS_HPP

#include <string>
#include <unordered_map>
#include <optional>
#include <functional>

struct Constant {
	std::string module;
    float defaultValue;
    float value;
    float buffer;
    float minValue;
    float maxValue;
    std::function<void(float)> onChange;

    void set();
};

void addConstant(const std::string& module, const std::string& name, float defaultValue,
    float minValue, float maxValue, std::function<void(float)>&& onChange) noexcept;
void resetConstant(const std::string& name) noexcept;
std::optional<float> getConstantValue(const std::string& name) noexcept;
std::unordered_map<std::string, Constant>& getAllConstants() noexcept;
std::optional<float*> getConstantBuffer(const std::string& name) noexcept;
void removeModuleConstants(const std::string& module) noexcept;

#endif