#ifndef PSIM_NUMERIC_TYPE_HPP
#define PSIM_NUMERIC_TYPE_HPP

#include <type_traits>

template<typename T>
concept NumericType = std::is_integral_v<T> || std::is_floating_point_v<T>;

#endif