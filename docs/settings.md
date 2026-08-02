# Adding Settings Options And Writing / Reading Configs
For data that you want to persist between PSim sessions, you can add
options in the settings menu and read / write configs to disk.
## Adding A Toggle Switch In Settings
You can register a switch with the ```registerSwitch``` function which accepts
the module, the name of the switch, a boolean default value, and a callback
for when the value is changed (the callback function accepts a boolean as the
new value).

## Reading And Writing Configs
You can save data to disk using ```writeConfig``` and access it with ```readConfig```.
```writeConfig``` accepts the module, the config's name, and the value to save.
```readConfig``` accepts the module and the config's name, and returns a ```std::optional<T>```
where ```T``` is the type of the config. Both of these functions are templated and
should be explicitly templated to maintain binary integrity and avoid implicit use
of the wrong type in the template.

## Example
Below is an example code snippet for a module which saves the value from the switch to disk.
```cpp
std::string module{"Switch Example"};
registerModule(module);
constexpr bool defaultValue = true;
bool value = defaultValue;
auto savedValue = readConfig<bool>(module, "Config");
if (savedValue.has_value()) {
    value = savedValue;
}
else {
    writeConfig<bool>(module, "Config", value);
}
registerSwitch(module, "Toggle Config", value, [](bool newValue) {
    writeConfig<bool>(module, "Config", newValue);
})
```