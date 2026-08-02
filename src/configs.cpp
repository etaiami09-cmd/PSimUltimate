#include "configs.hpp"

void setConfigValueVariadicLength(const std::string& name, const void* data, size_t length) {
    auto appData = getAppdataFolder();
    if (!appData.has_value()) {return;}
    auto configsFolder = *appData / "settings";
    auto file = std::ofstream(configsFolder / name, std::ios::binary);
    if (!file) {
        pushPopUpAlert(std::format("Could not open config file '{}'.", configsFolder / name));
        return;
    }
    file.write(static_cast<const char*>(data), static_cast<ssize_t>(length));
    if (file.fail()) {
        pushPopUpAlert(std::format(
            "Unknown error: Failed to write to config file '{}'.",
            configsFolder / name));
        return;
    }
}

bool readConfigValueVariadicLength(const std::string& name, void* data, size_t length) {
    auto appData = getAppdataFolder();
    if (!appData.has_value()) {
        pushPopUpAlert(std::format("Could not open config file '{}'.", name));
        return false;
    }
    auto configsFolder = *appData / "settings";
	auto filePath = configsFolder / name;
	if (!fs::exists(filePath)) {return false;}
	auto file = std::ifstream(configsFolder / name, std::ios::binary);
    if (!file) {
        pushPopUpAlert(std::format("Could not open config file '{}'.", name));
        return false;
    }
    file.read(static_cast<char*>(data), static_cast<ssize_t>(length));
    if (file.fail()) {
        pushPopUpAlert(std::format(
            "Unknown error: Failed to read from config file '{}'.",
            configsFolder / name));
        return false;
    }
    return true;
}