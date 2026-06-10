#ifndef PSIM_VEC2_HPP
#define PSIM_VEC2_HPP

#include "numeric_type.hpp"
#include <concepts>

template<NumericType T>
struct Vec2 {
    T x;
    T y;
    constexpr Vec2() noexcept requires std::constructible_from<T>;
    constexpr Vec2(T x, T y) noexcept;

    [[nodiscard]] constexpr Vec2<T> operator+(Vec2<T> other) const noexcept;
    [[nodiscard]] constexpr Vec2<T> operator-(Vec2<T> other) const noexcept;
    
    [[nodiscard]] constexpr Vec2<T> operator*(NumericType auto factor) const noexcept;
    
    [[nodiscard]] constexpr Vec2<T> operator/(NumericType auto dividor) const noexcept;
    
    constexpr void operator+=(Vec2<T> other) noexcept;
    constexpr void operator-=(Vec2<T> other) noexcept;
    constexpr void operator*=(NumericType auto factor) noexcept;
    constexpr void operator/=(NumericType auto dividor) noexcept;

    [[nodiscard]] constexpr auto magnitude() const noexcept;
    [[nodiscard]] constexpr auto magnitude_squared() const noexcept;
};


namespace PSimImpl {
template<NumericType E, typename T>
struct VectorOperators {
    [[nodiscard]] constexpr T operator+(T other) const noexcept {
        auto result = static_cast<const Vec2<E>&>(static_cast<const T&>(*this))
            + static_cast<Vec2<E>&>(other);
        return T(result.x, result.y);
    }
    [[nodiscard]] constexpr T operator-(T other) const noexcept {
        auto result = static_cast<const Vec2<E>&>(static_cast<const T&>(*this))
            - static_cast<Vec2<E>&>(other);
        return T(result.x, result.y);
    }
};
} // namespace PSimImpl

#include "inlines/Vec2-impl.hpp"

#endif