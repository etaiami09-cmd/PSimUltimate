#include "psim_module_api.hpp"
#include "raylib.h"

void openLink(const std::string& url) {
#if defined(_WIN32)
	std::string command = "start " + url;
	std::system(command.c_str());
#elif defined(__APPLE__)
	std::string command = "open " + url;
	std::system(command.c_str());
#else
	std::string command = "xdg-open " + url;
	std::system(command.c_str());
#endif
}

void initializePSimModule() {
    std::string module{"Study Links"};
	registerModule(module, false);

	registerTopMenuButton(module, "Physics Classroom", []() {
		openLink("https://www.physicsclassroom.com/tutorial");
	});
	registerTopMenuButton(module, "OpenStax Volume 1", []() {
		openLink("https://openstax.org/books/university-physics-volume-1/pages/preface");
	});
	registerTopMenuButton(module, "Khan Academy Physics", []() {
		openLink("https://www.khanacademy.org/science/physics/forces-newtons-laws");
	});
	registerTopMenuButton(module, "PhET Interactive Simulations", []() {
		openLink("https://phet.colorado.edu/en/about");
	});
}