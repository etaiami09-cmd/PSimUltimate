# Particle Simulator Ultimate - A Simple & Extensible Particle Simulator

![PSim logo](https://etaiami09-cmd.github.io/psim-assets/logo_transparent.png)

PSim is a particle simulator built to visualize how different forces cause particles to interact. Spawn particles, enable and disable forces, tweak settings and physical constants, save and load simulation states, and build your own modules with custom forces.

![PSim demo 1](https://etaiami09-cmd.github.io/psim-assets/demo-image.png)
![PSim demo 2](https://etaiami09-cmd.github.io/psim-assets/demo2.png)
![PSim demo 3](https://etaiami09-cmd.github.io/psim-assets/demo3.png)

## Try It Out

The latest release, with an installer, can be found on the [Releases page](https://github.com/etaiami09-cmd/PSimUltimate/releases). Simply follow the instructions in the install wizard and the app will appear in your start menu.

### Platform Support

| Platform | Status | How to get it |
| --- | --- | --- |
| Windows x64 | Official builds | Download the installer from [Releases](https://github.com/etaiami09-cmd/PSimUltimate/releases) |
| Linux | Build from source — verified on WSL2 | Requires a C++26-compliant toolchain - See [Compiling the Project](#compiling-the-project) |
| macOS | Build from source — untested | Requires a C++26-compliant toolchain — see [Compiling the Project](#compiling-the-project) |

## Features

- Responsive GUI with a clean, minimal layout
- Save and open simulation states from files
- Heavy dynamic module support, with 3 example DLLs included in the project's source code
- Built-in electrostatic and gravity modules

## Extensible By Design

PSimUltimate is written using a simple tech stack (modern C++ with ImGui and Raylib) and designed as an extensibility-first product. Anyone, even someone with very little experience in C++, can follow the tutorials and create their own module — the API is simple to understand and intuitive to use.

## Cross-Platform Compatibility

The shipped binaries are compiled for Windows x64, but PSimUltimate is designed to work on Linux and macOS as well. Follow the compiling instructions below to build it on your platform — note that this requires a C++26-compliant toolchain, which isn't yet available everywhere.

## License

This project is published under the MIT license, which can be viewed in `License.txt`.

## Stardance

This project was written over the summer as part of the Stardance program. The final stretch of development was less about adding features and more about polishing the existing architecture for release ahead of the program's deadline. PSim is polished now, and I hope others enjoy using it as much as I enjoyed building it.

## Compiling the Project

### Prerequisites

- [CMake](https://cmake.org/) 3.30 or newer
- A C++ compiler updated to C++26, and a C++26-compliant libc++
- Git (required by CMake's FetchContent to download raylib)
- An internet connection for the first build (raylib is fetched automatically)

### Build Steps

```
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

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
.\build\Release\PSimUlt.exe
```

In order to build the 3 modules that ship with PSim, you can read the build instructions in `docs/first_module.md` and apply them to the modules in `extra_modules`.

## A Note On AI Use

For me, the goal of this project was to learn, and that I did. I studied and gained experience in many things. I consulted with AI throughout the development process, including getting feedback on some devlogs, this README.md, and the 1.0 release notes. I also used AI in a few debug sessions which were too frustrating to be worth learning from. I didn't use AI for the project's code, with one exception — `CMakeLists.txt`, the project's build script, which I had AI help write and edit before I got the hang of CMake.

## Credits

This project heavily utilizes several incredible open source libraries: Raylib, rlImGui, nlohmann/json, and slightly altered versions of portable-file-dialogs and Dead ImGui.

This project also uses Catch2 for testing, though the tests are not very exhaustive.
