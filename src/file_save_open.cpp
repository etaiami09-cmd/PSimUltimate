#include <filesystem>

#include "confirm_action.hpp"
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
    if (file.size() == 0) {return;}
	if (!file.ends_with(".json")) {
		file += ".json";
	}
    if (std::filesystem::exists(file)) {
		confirmAction(std::format("Save state to {}", file),
			std::format("File {} already exists. Are you sure you want to override it?"
				" This may lead to loss of data.", file),
				[file]() {
					serializeState(file);
				});
	}
	else {
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
    	auto filePath = file[0];
    	confirmAction("Open Saved Simulation State",
    		"Are you sure you want to load this save file? This will override any unsaved progress.",
    		[filePath]() {
    			deserializeState(filePath);
    		});
    }
}