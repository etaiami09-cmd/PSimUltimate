#ifndef PSIM_SERIALIZER_HPP
#define PSIM_SERIALIZER_HPP

#include <string>

void serializeState(const std::string& path);
void deserializeState(const std::string& path);

#endif