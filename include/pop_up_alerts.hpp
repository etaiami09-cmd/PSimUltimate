#ifndef PSIM_POP_UP_ALERTS_HPP
#define PSIM_POP_UP_ALERTS_HPP

#include <string>

void pushPopUpAlert(const std::string& text);

void displayPopups();

int getMaxNotifications();

void setMaxNotifications(int newMax);

#endif