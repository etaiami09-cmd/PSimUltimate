# Adding Keybinds And Buttons To Your Module
Keybinds and top menu buttons are a good way to let the user interact
manually with parts of your module. Keybinds are a collection of
keyboard keys and mouse buttons that, when pressed all at once, will execute
a callback function exactly once, until the next time all those keys are pressed at once.
Top menu buttons add a button to the ```Modules``` tab of
PSim's top menu and execute a callback when that button is pressed.

## Registering Keybinds
Registering a keybind can be done through the ```registerKeybind``` function
which accepts the module's name, the keybind's name (ideally a very short
description of what it does), and ```std::span``` objects containing the mouse
buttons and keyboard keys the keybind requires. These just use the normal
Raylib ```KeyboardKey``` and ```MouseButton``` enums. The last thing
```registerKeybind``` needs passed is the actual callback that gets called
when the keybind is activated.

## Registering Top Menu Buttons
This is done with ```registerTopMenuButton``` which accepts the module's name,
the button's name, and the callback.

## Example
Below is a code snippet showing an example module with buttons that run the ```doSomething``` function.
```cpp
std::string module{"Something"};
registerModule(module);
std::vector<KeyboardKey> keys = {KEY_LEFT_CONTROL, KEY_Z};
registerKeybind(module, "Do Something", keys, {}, doSomething);
registerTopMenuButton(module, "Do Something", doSomething);
```