# Displaying Error Messages / Alerts
When something goes wrong, it should be immediately clear to the user
that the creator of the module considered a potential edge case that can't
be handled properly. This is why PSim's module API includes a way to
display a pop-up message that will appear in the bottom-right corner
of the screen until the user dismisses it.
## Example
Below is a code snippet showing a module that alerts the user as
soon as it is loaded:
```cpp
std::string module{"Alert Module"};
registerModule(module);
/*
* ... register stuff
*/
alertUser(module, "Alert Module Loaded.");
```