#ifndef PSIM_CONFIRM_ACTION_HPP
#define PSIM_CONFIRM_ACTION_HPP

#include <functional>

void confirmAction(const std::string& actionName, const std::string& explanation,
	std::function<void()> action);
void drawConfirmationPopup();

#endif