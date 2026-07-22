#ifndef PSIM_SERIALIZER_HPP
#define PSIM_SERIALIZER_HPP

#include <string>

void serializeState(const std::string& path);
void deserializeState(const std::string& path);

void addModuleSerializer(const std::string& module, std::function<std::string()> serializer);
void addModuleDeserializer(const std::string& module, std::function<void(std::string)> deserializer);

void removeModuleSerializersAndDeserializers(const std::string& module) noexcept;

#endif