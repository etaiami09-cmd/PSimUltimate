# Creating Your First PSimUltimate Module
PSimUltimate has an extremely minimal API, so creating a working module
is a piece of cake. All you need to do is copy the contents of ```example_module```
into your project folder, fill in whatever you need in module.cpp and compile with
the instructions in this file.
## The Entry Point
The entry point for your module must be defined in ```module.cpp```
as ```void initializePSimModule()```, and this entry point will get called by
the main program when the module is loaded.
## Registering Your Module
Registering a module is simple, all you have to do is call ```void registerModule(const std::string& name)```
and your module will appear in the modules list. Then, whenever you register something
(in order to make your module actually do something), you pass the same name that you registered
the module with and PSimUltimate will automatically associate it with your module.
## Registering Callbacks
There are 4 main callbacks that you can register and will get called every frame:
* ForceHandlers are callbacks that take in a ```std::span<Particle>``` and a ```std::span<Force>``` and is called at force-calculation time
* VelocityHandlers are callbacks that take only a ```std::span<Particle>``` and are called at velocity accumulation time
* PositionHandlers are callbacks that also take a ```std::span<Particle>``` and are called at position calculation time
* GraphicsHandlers are callbacks that take a ```std::span<Particle>``` and are called at render time

Each of these callback types has its own function for registering it:
* ForceHandler: ```void registerForce(const std::string&, ForceHandler)```
* VelocityHandler: ```void registerVelocityHandler(const std::string&, VelocityHandler)```
* PositionHandler: ```void registerPositionHandler(const std::string&, PositionHandler)```
* GraphicsHandler: ```void registerRenderer(const std::string& GraphicsHandler)```
## Registering Constants
You can register a constant by calling ```registerConstant(const std::string& module, const std::string& name, float defaultValue, float minValue, float maxValue, std::function<void(float)> onChange)```.

You can get the value of an constant by calling ```accessConstantValue(const std::string& name)``` which returns a ```std::optional<float>```

## Registering Attributes
You can register an attribute by calling ```registerParticleAttribute(const std::string& module, const std::string& name, float defaultValue, float minValue, float maxValue)```

You can access attribute values by calling ```accessAttribute(const std::string& name)``` which returns a ```std::optional<std::span<float>>```.

## Compiling Your Module
### Requirements
* CMake
* C++ compiler compatible with C++23
### Instructions
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
There will now be a file named ```libpsim_module_lib.dll``` in ```build```. This file can now be loaded with PSimUltimate.