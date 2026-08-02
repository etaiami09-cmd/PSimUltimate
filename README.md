# Particle Simulator Ultimate - A General Purpose Particle Simulator
![](https://github.com/user-attachments/assets/d9f4893b-f399-483a-8a96-977b5f171241)
## The Goal
This project aims to create a multimode particle simulation to with wide extensibility and module support.
The project should utilize ImGui to create a responsive UI and raylib to handle its own rendering needs.
## Try It Out
An alpha release with an installer can be found in [this link](https://github.com/etaiami09-cmd/PSimUltimate/releases/tag/v0.1.5). Simply follow the instructions in the install wizard and the app should pop up in your startup menu.
## Features
* Responsive GUI with thought-out layout
* Save and open simulation states from files
* Heavy dynamic module support
* Built-in electrostatic and gravity modules
## License
This project is published with the MIT license, which can be viewed in License.txt.
## Stardance
This project is written over summer as part of the Stardance program.
## Compiling the project

### Prerequisites

- [CMake](https://cmake.org/) 3.30 or newer
- A C++ compiler updated to C++26, and a C++26-compliant libc++.
- Git (required by CMake's FetchContent to download raylib)
- An internet connection for the first build (raylib is fetched automatically)

### Build steps

```sh
# 1. Clone the repository
git clone https://github.com/etaiami09-cmd/PSimUltimate.git
cd PSimUltimate

# 2. Configure (creates a build/ directory)
cmake -B build

# 3. Compile
cmake --build build

# 4. Run
.\build\Debug\PSimUlt.exe
```

On a Release build, pass `-DCMAKE_BUILD_TYPE=Release` during configuration:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
.\build\Release\PSimUlt.exe
```

> **Note:** The first configure step will clone the raylib repository from GitHub, which may take a moment depending on your connection speed.

## Credits
This project heavily utilizes 5 incredible open source libraries: Raylib, Dear ImGui, rlImGui, nlohmann/json, and a slightly altered version of portable-file-dialogs.

This project also uses Catch2 for testing.