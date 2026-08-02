#ifndef PSIM_WINDOW_TRANSFORMATIONS_HPP
#define PSIM_WINDOW_TRANSFORMATIONS_HPP

#include "vec2.hpp"

float getSystemScalingFactor();

void setScaling(float factor);
float getScaling();

void setOffset(Vec2<float> offset);
Vec2<float> getOffset();

void beginTransformationMode();
void endTransformationMode();

#endif