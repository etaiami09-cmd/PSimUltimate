#include "pop_up_alerts.hpp"
#include "version.hpp"

#include "app_data_utils.hpp"

std::optional<fs::path> getAppdataFolder() {
#if defined(_WIN32)
    const char* appData = std::getenv("APPDATA");
    if (appData == nullptr) {
        pushPopUpAlert("Could not access APPDATA environment variable.");
        return {};
    }
    return fs::path{appData} / (std::string{"PSimUltimate "} + PSimImpl::PSimAppVersion_);
#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        pushPopUpAlert("Could not access HOME environment variable.");
        return {};
    }
    return fs::path(home) / "Library" / "Application Support"
        / (std::string{"PSimUltimate "} + PSimImpl::PSimAppVersion_);
#else
    const char* xdgConfig = std::getenv("XDG_CONFIG_HOME");
    if (xdgConfig == nullptr) {
        pushPopUpAlert("Could not access XDG_CONFIG_HOME environment variable.");
        return {};
    }
    return fs::path(xdgConfig) / ".config" / (std::string{"PSimUltimate "} + PSimImpl::PSimAppVersion_);
#endif
}

void createAppDataFolderIfNotExists() {
    auto optionalPath = getAppdataFolder();
    if (!optionalPath.has_value()) {
        return;
    }
    fs::create_directories(*optionalPath / "settings");
	fs::create_directories(*optionalPath / "modules");
}
