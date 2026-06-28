#ifndef PSIM_GRAPHICAL_HPP
#define PSIM_GRAPHICAL_HPP

void startWindow();
void closeWindow();
void drawFrame();
float getPSimGUIWidth();

#include "inlines/graphical-impl.hpp"  // IWYU pragma: export

#endif