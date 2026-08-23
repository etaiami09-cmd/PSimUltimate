#ifndef PSIM_GRAPHICAL_HPP
#define PSIM_GRAPHICAL_HPP
#include "raylib.h"

void startWindow();
void closeWindow();
void drawFrame();
float getPSimGUIWidth();
const Texture2D& getLogo();

#include "graphical-impl.hpp"  // IWYU pragma: export

#endif