// NOLINTBEGIN

#include "Particle.hpp"
#include "Vec2.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include <type_traits>

TEST_CASE("Vec2 expression arithmetic functions correctly", "[math][vec2]") {
    Vec2<float> vec1{3.0f, 1.0f};
    Vec2<float> vec2{4.0f, 5.5f};

    SECTION("Addition works as expected") {
        auto result = vec1 + vec2;
        REQUIRE(result.x == vec1.x + vec2.x);
        REQUIRE(result.y == vec1.y + vec2.y);
    }

    SECTION("Subtraction works as expected") {
        auto result = vec1 - vec2;
        REQUIRE(result.x == vec1.x - vec2.x);
        REQUIRE(result.y == vec1.y - vec2.y);
    }

    SECTION("Multiplication works as expected") {
        auto factor = 3;
        auto result = vec1 * factor;
        REQUIRE(result.x == vec1.x * factor);
        REQUIRE(result.y == vec1.y * factor);
    }

    SECTION("Division works as expected") {
        auto dividor = 2.0f;
        auto result = vec1 / dividor;
        REQUIRE(result.x == vec1.x / dividor);
        REQUIRE(result.y == vec1.y / dividor);
    }

    SECTION("Negation works as expected") {
        auto result = -vec1;
        REQUIRE(result.x == -vec1.x);
        REQUIRE(result.y == -vec1.y);
    }
}

TEST_CASE("Magnitudes are correct", "[math][vec2]") {
    SECTION("Magnitude squared") {
        constexpr float error{0.0000001f};
        Vec2<float> vec{3.0f, 4.0f};
        REQUIRE_THAT(vec.magnitude_squared(), Catch::Matchers::WithinAbs(25.0f, error));
    }

    SECTION("Normal magnitude") {
        constexpr float error{0.0000001f};
        Vec2<float> vec{3.0f, 4.0f};
        REQUIRE_THAT(vec.magnitude(), Catch::Matchers::WithinAbs(5.0f, error));
    }
}

TEST_CASE("Vec2 inplace arithmetic functions correctly", "[math][vec2]") {
    Vec2<float> vec1{3.0f, 1.0f};
    Vec2<float> vec2{4.0f, 5.5f};

    SECTION("Inplace addition works as expected") {
        auto result = vec1;
        result += vec2;
        REQUIRE(result.x == vec1.x + vec2.x);
        REQUIRE(result.y == vec1.y + vec2.y);
    }

    SECTION("Inplace subtraction works as expected") {
        auto result = vec1;
        result -= vec2;
        REQUIRE(result.x == vec1.x - vec2.x);
        REQUIRE(result.y == vec1.y - vec2.y);
    }

    SECTION("Inplace multiplication works as expected") {
        auto factor = 3;
        auto result = vec1;
        result *= factor;
        REQUIRE(result.x == vec1.x * factor);
        REQUIRE(result.y == vec1.y * factor);
    }

    SECTION("Inplace Division works as expected") {
        auto dividor = 2.0f;
        auto result = vec1;
        result /= dividor;
        REQUIRE(result.x == vec1.x / dividor);
        REQUIRE(result.y == vec1.y / dividor);
    }
}

TEST_CASE("Vec2 variants produce correct types", "[math][vec2][physics]") {
    Vec2<float> base{1.0f, 3.0f};

    SECTION("Position is only declared explicitly") {
        Position pos1{base};
        auto pos2 = Position{base};
        auto pos3 = pos1;
        REQUIRE(std::is_same_v<decltype(pos1), Position>);
        REQUIRE(std::is_same_v<decltype(pos2), Position>);
        REQUIRE(std::is_same_v<decltype(pos3), Position>);
    }

    SECTION("Velocity is only declared explicitly") {
        Velocity vel1{base};
        auto vel2 = Velocity{base};
        auto vel3 = vel1;
        REQUIRE(std::is_same_v<decltype(vel1), Velocity>);
        REQUIRE(std::is_same_v<decltype(vel2), Velocity>);
        REQUIRE(std::is_same_v<decltype(vel3), Velocity>);
    }

    SECTION("Force is only declared explicitly") {
        Force f1{base};
        auto f2 = Force{base};
        auto f3 = f1;
        REQUIRE(std::is_same_v<decltype(f1), Force>);
        REQUIRE(std::is_same_v<decltype(f2), Force>);
        REQUIRE(std::is_same_v<decltype(f3), Force>);
    }
}

TEST_CASE("Vec2 variants can only be explicitly interchanged", "[math][vec2][physics]") {
    
    SECTION("Variants are not implicitly convertible") {
        REQUIRE(!std::is_convertible_v<Position, Velocity>);
        REQUIRE(!std::is_convertible_v<Velocity, Position>);
        REQUIRE(!std::is_convertible_v<Position, Force>);
        REQUIRE(!std::is_convertible_v<Force, Position>);
        REQUIRE(!std::is_convertible_v<Velocity, Force>);
        REQUIRE(!std::is_convertible_v<Force, Velocity>);
    }

    SECTION("Variants are explicitly convertible") {
        REQUIRE(std::is_constructible_v<Position, Velocity>);
        REQUIRE(std::is_constructible_v<Velocity, Position>);
        REQUIRE(std::is_constructible_v<Position, Force>);
        REQUIRE(std::is_constructible_v<Force, Position>);
        REQUIRE(std::is_constructible_v<Velocity, Force>);
        REQUIRE(std::is_constructible_v<Force, Velocity>);
    }

}

template<typename T>
concept HasNegationOverload = requires(T t) {
    -t;
};

template<typename T, typename V>
concept HasAdditionOverload =
    requires(T t, V v) {t + v;}
    || requires(T t, V v) {v + t;};

template<typename T, typename V>
concept HasSubtractionOverload =
    requires(T t, V v) {t - v;}
    || requires(T t, V v) {v - t;};

template<typename T>
concept HasMultiplicationOverload =
    requires(T t, float v) {t * v;}
    || requires(T t, float v) {v * t;};

template<typename T>
concept HasDivisionOverload = requires(T t, float v) {t / v;};

template<typename T>
concept HasInplaceOverloads = requires(T t1, T t2, float v) {
    t1 += t2;
    t1 -= t2;
    t1 *= v;
    t2 /= v;
};


TEST_CASE("Vec2 variants arithmetic is correct", "[math][vec2][physics]") {
    SECTION("Addition only works within the same type") {
        REQUIRE(!HasAdditionOverload<Force, Position>);
        REQUIRE(!HasAdditionOverload<Force, Velocity>);
        REQUIRE(!HasAdditionOverload<Velocity, Position>);
        REQUIRE(HasAdditionOverload<Force, Force>);
        REQUIRE(HasAdditionOverload<Velocity, Velocity>);
        REQUIRE(HasAdditionOverload<Position, Position>);
    }

    SECTION("Subtraction only works within the same type") {
        REQUIRE(!HasSubtractionOverload<Force, Position>);
        REQUIRE(!HasSubtractionOverload<Force, Velocity>);
        REQUIRE(!HasSubtractionOverload<Velocity, Position>);
        REQUIRE(HasSubtractionOverload<Force, Force>);
        REQUIRE(HasSubtractionOverload<Velocity, Velocity>);
        REQUIRE(HasSubtractionOverload<Position, Position>);
    }

    SECTION("Inplace overloads implemented") {
        REQUIRE(HasInplaceOverloads<Position>);
        REQUIRE(HasInplaceOverloads<Velocity>);
        REQUIRE(HasInplaceOverloads<Force>);
    }

    SECTION("Negation operator overloaded") {
        REQUIRE(HasNegationOverload<Position>);
        REQUIRE(HasNegationOverload<Velocity>);
        REQUIRE(HasNegationOverload<Force>);
    }
}

// NOLINTEND