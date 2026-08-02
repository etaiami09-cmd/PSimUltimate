#ifndef PSIM_CONFIGS_HPP
#define PSIM_CONFIGS_HPP

#include <fstream>
#include <string>

#include "app_data_utils.hpp"
#include "pop_up_alerts.hpp"

void setConfigValueVariadicLength(const std::string& name, const void* data, size_t length);

bool readConfigValueVariadicLength(const std::string& name, void* data, size_t length);

template<typename... Ts>
void setConfigValue(const std::string& name, Ts... values) {
    auto appData = getAppdataFolder();
    if (!appData.has_value()) {return;}
    auto configsFolder = *appData / "settings";
    auto file = std::ofstream(configsFolder / name, std::ios::trunc | std::ios::binary);
    if (!file) {
        pushPopUpAlert(std::format("Could not open config file '{}'.", configsFolder / name));
        return;
    }
    (file.write(reinterpret_cast<const char*>(&values), sizeof(values)), ...);
    if (file.fail()) {
        pushPopUpAlert(std::format(
            "Unknown error: Failed to write to config file '{}'.",
            configsFolder / name));
        return;
    }
}

template<typename... Ts>
std::optional<std::tuple<Ts...>> getConfigValue(const std::string& name) {
    auto appData = getAppdataFolder();
    if (!appData.has_value()) {return {};}
    auto configsFolder = *appData / "settings";
	auto filePath = configsFolder / name;
	if (!fs::exists(filePath)) {return {};}
    auto file = std::ifstream(filePath, std::ios::binary);
    if (!file) {
        pushPopUpAlert(std::format("Could not open config file '{}'.", configsFolder / name));
        return {};
    }
    std::tuple<Ts...> values{};
    [&file, &values]<size_t... Is>(std::index_sequence<Is...>) {
        (file.read(reinterpret_cast<char*>(&std::get<Is>(values)),
            sizeof(std::get<Is>(values))), ...);
    }(std::make_index_sequence<sizeof...(Ts)>{});
    if (file.fail()) {
        pushPopUpAlert(std::format(
            "Unknown error: Failed to read from config file '{}'.",
            configsFolder / name));
        return {};
    }
    return values;
}

#endif