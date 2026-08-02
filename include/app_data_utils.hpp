#ifndef PSIM_APP_DATA_UTILS_HPP
#define PSIM_APP_DATA_UTILS_HPP

#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

std::optional<fs::path> getAppdataFolder();
void createAppDataFolderIfNotExists();

#endif