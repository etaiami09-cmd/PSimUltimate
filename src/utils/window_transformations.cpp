#include "raylib.h"

#include "window_transformations.hpp"

namespace {

float scalingFactor{1.0f};
Vec2<float> cameraOffset{};

} // namespace

float getSystemScalingFactor() {
	return GetWindowScaleDPI().x;
}

void setScaling(float factor) {
	scalingFactor = factor;
}

float getScaling() {
	return scalingFactor;
}

void setOffset(Vec2<float> offset) {
	cameraOffset = offset;
}

Vec2<float> getOffset() {
	return cameraOffset;
}

void beginTransformationMode() {
	Camera2D camera{};
	camera.offset = {0, 0};
	camera.rotation = 0;
	camera.target = {-cameraOffset.x, -cameraOffset.y};
	camera.zoom = scalingFactor;
	BeginMode2D(camera);
}

void endTransformationMode() {
	EndMode2D();
}
