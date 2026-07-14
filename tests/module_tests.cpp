// NOLINTBEGIN

#include "Particle.hpp"
#include "attributes.hpp"
#include "catch2/catch_test_macros.hpp"
#include "constants.hpp"
#include "load_module.hpp"
#include "module.hpp"
#include "particles.hpp"
#include "psim_module_api.hpp"
#include <cassert>

TEST_CASE("Modules do not double", "[modules]") {
    loadBuiltinModuleTable();
    SECTION("Check") {
        registerModule("test");
        registerModule("test");
        REQUIRE(getModules().size() == 1);
    }
}

TEST_CASE("Inactive modules are skipped", "[modules]") {
    loadBuiltinModuleTable();
    SECTION("Setup") {
        loadBuiltinModuleTable();
    }
    
    // SECTION("Check") {
    //     registerModule("test");
    //     registerForce("test", [](auto particles, auto forces) {
    //         REQUIRE(getModules()[0].active);
    //     });
    //     registerVelocityHandler("test", [](auto particles) {
    //         REQUIRE(getModules()[0].active);
    //     });
    //     registerPositionHandler("test", [](auto particles) {
    //         REQUIRE(getModules()[0].active);
    //     });

    //     getModules()[0].active = false;

    //     Particles::add(Position{0.0f, 0.0f}, Velocity{0.0f, 0.0f}, 10.0f, 1.0f, {});

    //     Particles::tick(1.0f);

    //     getModules()[0].active = true;

    //     Particles::tick(1.0f);
    // }
}

TEST_CASE("Constants work", "[modules]") {
    loadBuiltinModuleTable();
    registerModule("test");

    SECTION("Registering a constant works") {
        registerConstant("test", "test_const", 0, -1, 1, [](float newValue) {
            REQUIRE(newValue == 0);
        });
        auto val = getConstantValue("test_const");
        REQUIRE(val.has_value());
        REQUIRE(val.value() == 0);
        resetConstant("test_const");
    }
}

TEST_CASE("Attributes work", "[modules]") {
    loadBuiltinModuleTable();
    registerModule("test");

    SECTION("Attributes") {
        registerParticleAttribute("test", "test_att", 0, -1, 1);
        registerParticleAttribute("test", "test_att1", 0, -1, 1);
        Particles::reset();
        Particles::add({0, 0}, {0, 0}, 10, 1, {1, -1});
        Particles::add({0, 0}, {0, 0}, 10, 1, {0.4, 0.4});
        const auto& attvalues = getAttributeByName("test_att").value().values;
        REQUIRE(attvalues[0] == 1);
        REQUIRE(attvalues[1] == 0.4f);
        const auto& att1values = getAttributeByName("test_att1").value().values;
        REQUIRE(att1values[0] == -1);
        REQUIRE(att1values[1] == 0.4f);
        REQUIRE(getAttributes().size() == 2);
        REQUIRE(attvalues.size() == 2);
        REQUIRE(att1values.size() == 2);
        REQUIRE(getAttributeByName("test_att").value().defaultValue == 0);
        REQUIRE(getAttributeByName("test_att").value().minValue == -1);
        REQUIRE(getAttributeByName("test_att").value().maxValue == 1);
        REQUIRE(getAttributeByName("test_att1").value().defaultValue == 0);
        REQUIRE(getAttributeByName("test_att1").value().minValue == -1);
        REQUIRE(getAttributeByName("test_att1").value().maxValue == 1);
    }
}

// NOLINTEND