# Particle Simulator Ultimate - A General Purpose Particle Simulator
## The Goal
This project aims to create a multi-mode particle simulation to simulate electrostatics, gravity, and chemical bonds.
The project should utilize ImGui to create a responsive UI and raylib to handle its own rendering needs.
## License
This project is published with the MIT license, which can be viewed in License.txt.
## Stardance
This project is written over summer as part of the Stardance program.
## Compiling the project

### Prerequisites

- [CMake](https://cmake.org/) 3.30 or newer
- [LLVM/Clang](https://releases.llvm.org/) installed to `C:\Program Files\LLVM\` (the path is hardcoded in `CMakeLists.txt`)
- Git (required by CMake's FetchContent to download raylib)
- An internet connection for the first build (raylib is fetched automatically)

### Build steps

```sh
# 1. Clone the repository
git clone <repo-url>
cd ParticleSimulatorUltimate

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