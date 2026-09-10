#ifndef PSIM_PARTICLE_CREATION_GUI_HPP
#define PSIM_PARTICLE_CREATION_GUI_HPP

void vecInput(const char* name, float* xTarget, float* yTarget);
bool valueInput(const char* name, float* target, float minValue, float maxValue);
void drawParticleCreationGUI();

bool shouldDrawTarget();
void setShouldDrawTarget(bool newValue);

std::array<float, 4> getNewParticleTargetColor();
void setNewParticleTargetColor(std::array<float, 4> newColor);
int getNewParticleTargetLength();
void setNewParticleTargetLength(int newLength);
int getNewParticleTargetWidth();
void setNewParticleTargetWidth(int newWidth);
void disableParticlePositionSelection();

#endif