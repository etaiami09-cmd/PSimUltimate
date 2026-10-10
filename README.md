# Particle Simulator Ultimate - A Simple & Extensible Particle Simulator

![PSim logo](https://etaiami09-cmd.github.io/psim-assets/logo_transparent.png)

PSimUltimate is a particle simulator that allows you to see how particles interact with different forces and each other,
by allowing you to spawn particles, enable and disable forces/modules to see how the interactions change, save and load
state for quick replaying, and build your own modules with custom forces, buttons, and ways to interact with the
application.

![PSim demo 1](https://etaiami09-cmd.github.io/psim-assets/demo-image.png)
![PSim demo 2](https://etaiami09-cmd.github.io/psim-assets/demo2.png)
![PSim demo 3](https://etaiami09-cmd.github.io/psim-assets/demo3.png)

## Try It Out

The latest release, shipped with a Windows installer and 3 compiled modules, can be found on
the [Releases Page](https://github.com/etaiami09-cmd/PSimUltimate/releases). Simply follow the instructions in the
installer, and PSim will be ready for you to use.

Note: The shipped binaries are only for Windows, so Linux and Mac users must compile PSim on their own.

### Platform Support

| Platform | Support                 | How to get it             |
|----------|-------------------------|---------------------------|
| Windows  | Complete                | Official Releases Page    |
| Linux    | Verified working on WSL | Must be compiled manually |
| Mac      | Not tested              | Must be compiled manually |

## Features
- Responsive, minimal and clean GUI
- Save and open states from files
- Heavy dynamically-loaded module support (see - Extensibility)
- Builtin electrostatic and gravity modules

## Extensibility
PSimUltimate is designed with extensibility first - A lot of time has been spent engineering a system that allows people with basic C++ knowledge to
create their own modules and load them dynamically at runtime. PSim even ships with 3 compiled DLLs ready to go and try, ones that were not "good"
or "essential" enough to be included in the main binary itself, but that provide a nice preview into what can be done with modules.

To get started, visit the docs folder in the source tree, and read ```first_module.md```.

## License

This project is published under the MIT license (see ```License.txt```).

## Stardance

This project was made for the Stardance program.

## Compiling The Project

### Requirements
* CMake 3.30 or newer (or 3.5 if you're only compiling modules)
* A C++ compiler compatible with C++26 and a C++26 standard library implementation
* Git

### Steps
 
Clone the repository:
```bash
git clone https://github.com/etaiami09-cmd/PSimUltimate.git
cd PSimUltimate
```
Compile
```bash
cmake -B build
cmake --build build
```
For a Release build:
```bash
-cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
To build modules, including the modules in ```extra-modules/```, follow the instructions in ```docs/first_modules.md```.

## A Note On AI Use
The goal of this project was to learn, and I did learn quite a lot writing it. I never used AI to write any actual C++ code, only for CMake before I got the hang of it. I also used AI for some advice on
file structure, seeing how some APIs work (example: platform-specific APIs for the modules), and some debugging when it got too difficult.
I also used AI for advice on how to write some parts of my devlogs, but never directly wrote with it, and mainly used it for correcting grammar and spelling errors.

## Credits
This project heavily depends on a few amazing open-source libraries: Raylib, rlImGUi, nlohmann-json, and slightly altered versions I made of portable-file-dialogs and Dear ImGui.

I also used Catch2 for testing, but I hardly wrote any tests unfortunately.
#
#
#
#
#
