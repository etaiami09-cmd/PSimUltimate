#ifndef PSIM_ATTRIBUTES_HPP
#define PSIM_ATTRIBUTES_HPP

#include <string>
#include <span>
#include <vector>
#include <cstddef>

struct Attribute {
    std::string name;
    float defaultValue;
    float minValue;
    float maxValue;
    std::vector<float> values;
};

void addAttribute(const std::string& name, float defaultValue,
    float minValue, float maxValue) noexcept;
std::span<const Attribute> getAttributes() noexcept;
const Attribute& getAttributeByName(const std::string& name);
void addNewParticleAttributes(const std::vector<float>& values) noexcept;
void setParticleAttributes(size_t index, const std::vector<float>& values) noexcept;
void clearAttributeValues() noexcept;
void resizeAttributeValues(size_t newSize) noexcept;
void pushDefaultParticleAttributes() noexcept;

#endif