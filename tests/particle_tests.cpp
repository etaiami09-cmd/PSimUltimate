// NOLINTBEGIN

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "Particle.hpp"
#include "load_module.hpp"
#include "module.hpp"
#include "particles.hpp"
#include "psim_module_api.hpp"

TEST_CASE("Particles tick correctly", "[physics][particles][math]") {
    SECTION("Tick tests") {
        {
            Particle p{Position{0, 0}, Velocity{1, 2}, 3, 3};
            constexpr float error{0.0000001f};
            p.tick(1.0f);
            REQUIRE_THAT(p.getPosition().x, Catch::Matchers::WithinAbs(1, error));
            REQUIRE_THAT(p.getPosition().y, Catch::Matchers::WithinAbs(2, error));
        }
        {
            Particle p{Position{0, 0}, Velocity{1, 2}, 3, 3};
            constexpr float error{0.0000001f};
            p.tick(0.016f);
            REQUIRE_THAT(p.getPosition().x, Catch::Matchers::WithinAbs(0.016f, error));
            REQUIRE_THAT(p.getPosition().y, Catch::Matchers::WithinAbs(0.032f, error));
        }
    }
}

TEST_CASE("Particles namespace ticks cleanly", "[physics][particles][math]") {

    SECTION("Tick test") {
        constexpr float error{0.0000001f};
        Particles::reset();
        Particles::add(Position{0.0f, 0.0f}, Velocity{1.0f, 2.0f}, 5, 3);
        Particles::add(Position{3.4f, 2.0f}, Velocity{0.0f, 1.5f}, 5, 10);
        Particles::tick(1.0);

        const auto& p1 = Particles::get()[0];
        const auto& p2 = Particles::get()[1];

        REQUIRE_THAT(p1.getPosition().x, Catch::Matchers::WithinAbs(1.0f, error));
        REQUIRE_THAT(p1.getPosition().y, Catch::Matchers::WithinAbs(2.0f, error));
        REQUIRE_THAT(p2.getPosition().x, Catch::Matchers::WithinAbs(3.4f, error));
        REQUIRE_THAT(p2.getPosition().y, Catch::Matchers::WithinAbs(3.5f, error));

        Particles::tick(0.016f);

        REQUIRE_THAT(p1.getPosition().x, Catch::Matchers::WithinAbs(1.016f, error));
        REQUIRE_THAT(p1.getPosition().y, Catch::Matchers::WithinAbs(2.032f, error));
        REQUIRE_THAT(p2.getPosition().x, Catch::Matchers::WithinAbs(3.4f, error));
        REQUIRE_THAT(p2.getPosition().y, Catch::Matchers::WithinAbs(3.524f, error));
    }

}

TEST_CASE("Particles namespace accumulates forces correctly", "[physics][particles][math]") {
    
    SECTION("Setup") {
        loadBuiltinModuleTable();
        registerModule("test");
        registerForce("test", [](const auto particles, auto forces) {
            for (auto& force : forces) {
                force += Force{1.0f, 2.0f};
            }
        });
        getModules()[0].active = true;
    }

    SECTION("Single particle") {
        constexpr float error{0.0000001f};
        Particles::reset();
        Particles::add(Position{0.0f, 0.0f}, Velocity{0.0f, 0.0f}, 10, 1);
        Particles::tick(1.0f);
        const auto& p = Particles::get()[0];
        REQUIRE_THAT(p.getPosition().x, Catch::Matchers::WithinAbs(1.0f, error));
        REQUIRE_THAT(p.getPosition().y, Catch::Matchers::WithinAbs(2.0f, error));
    }

    SECTION("Multiple Particles") {
        constexpr float error{0.0000001f};
        Particles::reset();
        Particles::add(Position{0.0f, 0.0f}, Velocity{0.0f, 0.0f}, 10, 1);
        Particles::add(Position{1.0f, 1.0f}, Velocity{1.0f, 1.0f}, 10, 2);
        Particles::tick(1.0f);
        const auto& p1 = Particles::get()[0];
        const auto& p2 = Particles::get()[1];
        REQUIRE_THAT(p1.getPosition().x, Catch::Matchers::WithinAbs(1.0f, error));
        REQUIRE_THAT(p1.getPosition().y, Catch::Matchers::WithinAbs(2.0f, error));
        REQUIRE_THAT(p2.getPosition().x, Catch::Matchers::WithinAbs(2.5f, error));
        REQUIRE_THAT(p2.getPosition().y, Catch::Matchers::WithinAbs(3.0f, error));
    }
}

TEST_CASE("Particles namespace handles particles correctly", "[particles][memory]") {
    SECTION("One thousand particles") {
        Particles::reset();
        for (int i = 0; i < 1000; i++) {
            Particles::add(Position{1.0f, 1.0f}, Velocity{0.0f, 0.0f}, 10, 1);
        }
        REQUIRE(Particles::get().size() == 1000);
    }

    SECTION("Reset works") {
        Particles::reset();
        for (int i = 0; i < 1000; i++) {
            Particles::add(Position{1.0f, 1.0f}, Velocity{0.0f, 0.0f}, 10, 1);
        }
        Particles::reset();
        REQUIRE(Particles::get().empty());
    }
}

// NOLINTEND