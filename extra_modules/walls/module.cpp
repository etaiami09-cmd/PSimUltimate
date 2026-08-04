#include <algorithm>

#include "psim_module_api.hpp"
#include "raylib.h"

namespace {
bool shouldDrawWalls;
constexpr bool defaultShouldDrawWalls = true;

float topWall{};
float bottomWall{};
float leftWall{};
float rightWall{};
} // namespace

void initializePSimModule() {
	std::string module{"Walls"};
	registerModule(module);

	auto config = readConfig<bool>(module, "ShouldDrawWalls");
	if (!config.has_value()) {
		shouldDrawWalls = defaultShouldDrawWalls;
		writeConfig<bool>(module, "ShouldDrawWalls", defaultShouldDrawWalls);
	}
	else {
		shouldDrawWalls = config.value();
	}

	registerSwitch(module, "Draw Walls", shouldDrawWalls, [module](bool newValue) {
		shouldDrawWalls = newValue;
		writeConfig<bool>(module, "ShouldDrawWalls", shouldDrawWalls);
	});

	registerConstant(module, "Top Wall", topWall, std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::max(), [](float newValue) {
			topWall = newValue;
		});
	bottomWall = GetScreenHeight();
	registerConstant(module, "Bottom Wall", bottomWall, std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::max(), [](float newValue) {
			bottomWall = newValue;
		});
	registerConstant(module, "Left Wall", leftWall, std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::max(), [](float newValue) {
			leftWall = newValue;
		});
	rightWall = GetScreenWidth();
	registerConstant(module, "Right Wall", rightWall, std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::max(), [](float newValue) {
			rightWall = newValue;
		});

	registerPositionHandler(module, [](std::span<Particle> particles) {
		for (auto& particle : particles) {
			if (particle.getPosition().x - particle.getRadius() < leftWall
				|| particle.getPosition().x + particle.getRadius() > rightWall) {
				particle.setPosition(Position{
					std::clamp(particle.getPosition().x,
					leftWall + particle.getRadius(), rightWall - particle.getRadius()),
					particle.getPosition().y
				});
				particle.setVelocity(Velocity{
					-particle.getVelocity().x,
					particle.getVelocity().y
				});
			}
			if (particle.getPosition().y - particle.getRadius() < topWall
				|| particle.getPosition().y + particle.getRadius() > bottomWall) {
				particle.setPosition(Position{
					particle.getPosition().x,
					std::clamp(particle.getPosition().y,
						topWall + particle.getRadius(), bottomWall - particle.getRadius()),
				});
				particle.setVelocity(Velocity{
					particle.getVelocity().x,
					-particle.getVelocity().y
				});
			}
		}
	});

	constexpr float thickness = 10;
	constexpr Color wallColor = BLACK;

	registerRenderer(module, [](std::span<const Particle> particles) {
		if (!shouldDrawWalls) {return;}
		DrawLineEx({leftWall, topWall}, {rightWall, topWall},
			thickness, wallColor);
		DrawLineEx({leftWall, topWall}, {leftWall, bottomWall},
			thickness, wallColor);
		DrawLineEx({leftWall, bottomWall}, {rightWall, bottomWall},
			thickness, wallColor);
		DrawLineEx({rightWall, topWall}, {rightWall, bottomWall},
			thickness, wallColor);
	});
}