#include <cstddef>
#include <optional>

#include "raylib.h"

#include "Particle.hpp"
#include "particles.hpp"

#include "edit_particles_ui.hpp"

#include <format>
#include <ranges>

#include "attributes.hpp"
#include "confirm_action.hpp"
#include "graphical.hpp"
#include "particle_creation_gui.hpp"


namespace {
ssize_t selectedParticle = -1;

template<NumericType T>
T square(T value) {
	return value * value;
}

std::optional<size_t> findHoveredParticleIndex() {
	Vec2<float> offset{getPSimGUIWidth(), ImGui::GetFrameHeight()};
	auto result = std::ranges::find_if(Particles::getNonConst(),
	[&](const auto& particle) {
		return (square(GetMousePosition().x - offset.x - particle.getPosition().x)
			+ square(GetMousePosition().y - offset.y - particle.getPosition().y))
			<= square(particle.getRadius());
	});
	if (result == Particles::getNonConst().end()) {
		return std::nullopt;
	}
	return result - Particles::getNonConst().begin();
}

Position particlePosition;
Velocity particleVelocity;
float particleMass;
float particleRadius;
AttributeEditor attributeEditor;

bool shouldScrollY{false};
} // namespace

void runEditParticlesUI() {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		auto result = findHoveredParticleIndex();
		if (result.has_value()) {
			selectedParticle = result.value();
			const auto& particle = Particles::getNonConst()[selectedParticle];
			particlePosition = particle.getPosition();
			particleVelocity = particle.getVelocity();
			particleMass = particle.getMass();
			particleRadius = particle.getRadius();
			attributeEditor = AttributeEditor{
				static_cast<size_t>(selectedParticle),
				(getAttributes()
				| std::views::transform([=](const auto& attribute) {
				return attribute.values[selectedParticle];
				}) | std::ranges::to<std::vector>())
			};
			shouldScrollY = true;
		}
	}
	if (selectedParticle != -1) {
		if (Particles::get().size() <= selectedParticle) {
			selectedParticle = -1;
			return;
		}
		ImGui::SeparatorText("Edit Particle");
		ImGui::Spacing();
		vecInput("Position ", &particlePosition.x, &particlePosition.y);
		vecInput("Velocity ", &particleVelocity.x, &particleVelocity.y);
		valueInput("Radius ", &particleRadius, Particle::minRadius, Particle::maxRadius);
		valueInput("Mass ", &particleMass, Particle::minMass, Particle::maxMass);
		for (const auto& [i, attribute]: std::views::enumerate(getAttributes())) {
			if (valueInput(std::format("{} ", attribute.name).c_str(), &attributeEditor.values[i],
				attribute.minValue, attribute.maxValue)) {
				setParticleAttributes(attributeEditor);
			}
		}
		if (ImGui::Button("Delete Particle", ImVec2(0, 40))) {
			confirmAction("Delete Particle", "This action cannot be undone", [=]() {
				Particles::remove(selectedParticle);
				selectedParticle = -1;
			});
		}
		else {
			auto& particle = Particles::getNonConst()[selectedParticle];
			particle.setPosition(particlePosition);
			particle.setVelocity(particleVelocity);
			particle.setMass(particleMass);
			particle.setRadius(particleRadius);
		}
		if (ImGui::Button("Deselect Particle", ImVec2(0, 40))) {
			selectedParticle = -1;
		}
		if (shouldScrollY) {
			ImGui::SetScrollHereY();
			shouldScrollY = false;
		}
	}
}
