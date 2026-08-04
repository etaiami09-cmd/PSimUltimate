#ifndef PSIM_VEC2_IMPL_HPP
#define PSIM_VEC2_IMPL_HPP

#include <cmath>

#include "Vec2.hpp"

#include "numeric_type.hpp"

template<NumericType T>
constexpr Vec2<T>::Vec2() noexcept requires std::constructible_from<T>
: x{}, y{} {}
template<NumericType T>
constexpr Vec2<T>::Vec2(T x, T y) noexcept 
: x{x}, y{y} {}

template<NumericType T>
[[nodiscard]] constexpr Vec2<T> Vec2<T>::operator+(Vec2<T> other) const noexcept {
    return Vec2<T>{x + other.x, y+other.y};
}
template<NumericType T>
[[nodiscard]] constexpr Vec2<T> Vec2<T>::operator-(Vec2<T> other) const noexcept {
    return Vec2<T>{x - other.x, y-other.y};
}
template<NumericType T>
[[nodiscard]] constexpr Vec2<T> Vec2<T>::operator*(NumericType auto factor) const noexcept {
    return Vec2<T>{x * factor, y * factor};
}
template<NumericType T>
[[nodiscard]] constexpr Vec2<T> Vec2<T>::operator/(NumericType auto dividor) const noexcept {
    return Vec2<T>{x / dividor, y / dividor};
}
template<NumericType T>
[[nodiscard]] constexpr Vec2<T> Vec2<T>::operator-() const noexcept {
    return Vec2<T>{-x, -y};
}

template<NumericType T>
template<typename Child>
requires std::derived_from<Child, Vec2<T>>
constexpr Vec2<T>::operator Child() const {
    return Child{x, y};
}

template<NumericType T>
constexpr void Vec2<T>::operator+=(Vec2<T> other) noexcept {
    x += other.x;
    y += other.y;
}

template<NumericType T>
constexpr void Vec2<T>::operator-=(Vec2<T> other) noexcept {
    x -= other.x;
    y -= other.y;
}

template<NumericType T>
constexpr void Vec2<T>::operator*=(NumericType auto factor) noexcept {
    x *= factor;
    y *= factor;
}

template<NumericType T>
constexpr void Vec2<T>::operator/=(NumericType auto dividor) noexcept {
    x /= dividor;
    y /= dividor;
}

template<NumericType T>
[[nodiscard]] constexpr auto Vec2<T>::magnitude() const noexcept {
    return std::hypot(x, y);
}
template<NumericType T>
[[nodiscard]] constexpr auto Vec2<T>::magnitude_squared() const noexcept {
    return (x * x) + (y * y);
}

#endif