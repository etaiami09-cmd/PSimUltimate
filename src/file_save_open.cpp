#include "portable-file-dialogs.h"
#include "serializer.hpp"

#include "file_save_open.hpp"

void saveStateToFile() {
    auto fileDialog = pfd::save_file(
        "Save Simulation File",
        "",
        {"JSON Files", "*.json"},
        pfd::opt::none
    );
    auto file = fileDialog.result();
    if (!file.ends_with(".json")) {
        file += ".json";
    }
    if (file.size() != 0) {
        serializeState(file);
    }
}

void openStateFromFile() {
    auto fileDialog = pfd::open_file(
        "Open Simulation File",
        "",
        {"JSON Files", "*.json"},
        pfd::opt::none
    );
    auto file = fileDialog.result();
    if (!file.empty()) {
        deserializeState(file[0]);
    }
}