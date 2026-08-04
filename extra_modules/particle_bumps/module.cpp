#include "psim_module_api.hpp"
#include "raylib.h"

#include <algorithm>

template<typename T, typename E>
requires std::derived_from<T, Vec2<float>> && std::derived_from<E, Vec2<float>>
float dot(const T& vec1, const E& vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

void resolveCollision(Particle& a, Particle& b, float restitution) {
	constexpr float epsilon = 1e-6f;

	auto displacement = b.getPosition() - a.getPosition();
	float distance = displacement.magnitude();
	float radiusSum = a.getRadius() + b.getRadius();

	if (distance >= radiusSum) {
		return;
	}

	auto relativeVelocity = b.getVelocity() - a.getVelocity();

	auto separationNormal = distance > epsilon
		? displacement / distance
		: decltype(displacement){1.0f, 0.0f};

	float inverseMassA = 1.0f / a.getMass();
	float inverseMassB = 1.0f / b.getMass();
	float inverseMassSum = inverseMassA + inverseMassB;

	if (inverseMassSum <= epsilon) {
		return;
	}

	float velocityAlongNormal = dot(relativeVelocity, separationNormal);

	// Already separating along the normal—no impulse needed.
	if (velocityAlongNormal > 0.0f) {
		return;
	}

	restitution = std::clamp(restitution, 0.0f, 1.0f);

	float impulseMagnitude =
		-(1.0f + restitution) * velocityAlongNormal /
		inverseMassSum;

	auto impulse = separationNormal * impulseMagnitude;

	a.setVelocity(
		a.getVelocity() - Velocity{impulse} * inverseMassA
	);

	b.setVelocity(
		b.getVelocity() + Velocity{impulse} * inverseMassB
	);

	// Always separate them along their actual current center-to-center axis.
	float penetration = radiusSum - distance;

	auto correction =
		separationNormal * (penetration / inverseMassSum);

	a.setPosition(
		a.getPosition() - correction * inverseMassA
	);

	b.setPosition(
		b.getPosition() + correction * inverseMassB
	);
}

void initializePSimModule() {
    std::string module{"Bump Particles"};
	registerModule(module);

	registerPositionHandler(module, [](std::span<Particle> particles) {
		for (size_t i = 0; i < particles.size(); i++) {
			for (size_t j = i + 1; j < particles.size(); j++) {
				resolveCollision(particles[i], particles[j], 1.0f);
			}
		}
	});
}