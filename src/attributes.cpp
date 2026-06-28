#include <span>
#include <vector>
#include <string>
#include <algorithm>

#include "particles.hpp"

#include "attributes.hpp"

namespace {

std::vector<Attribute> attributes;

} // namespace

void addAttribute(const std::string& name, float defaultValue,
    float minValue, float maxValue) noexcept {
    attributes.emplace_back(
        name, defaultValue, minValue, maxValue,
        std::vector<float>(Particles::get().size(), defaultValue)
    );
}

std::span<const Attribute> getAttributes() noexcept {
    return attributes;
}

const Attribute& getAttributeByName(const std::string& name) {
    return *std::ranges::find_if(attributes,
        [&name](const auto& otherName) {return name == otherName;},
        &Attribute::name
    );
}

void addNewParticleAttributes(const std::vector<float>& values) noexcept {
    for (size_t i = 0; i < attributes.size(); i++) {
        attributes[i].values.push_back(values[i]);
    }
}

void setParticleAttributes(size_t index, const std::vector<float>& values) noexcept {
    if (index > Particles::get().size()) {return;}
    for (size_t i = 0; i < attributes.size(); i++) {
        attributes[i].values[index] = values[i];
    }
}

void clearAttributeValues() noexcept {
    for (auto& attribute : attributes) {
        attribute.values.clear();
    }
}

void resizeAttributeValues(size_t newSize) noexcept {
    for (auto& attribute : attributes) {
        attribute.values.resize(newSize);
    }
}

void pushDefaultParticleAttributes() noexcept {
    std::vector<float> newValues{};
    newValues.reserve(attributes.size());
    for (auto& attribute : attributes) {
        newValues.push_back(attribute.defaultValue);
    }
    addNewParticleAttributes(newValues);
}