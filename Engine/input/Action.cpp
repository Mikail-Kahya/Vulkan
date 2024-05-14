#include "Action.h"

using namespace mk;

bool Action::Triggered(const ControllerInput& controller, const KeyboardInput& keyboard) const
{
	switch (type)
	{
	case ActionType::down:
		return keyboard.ButtonDown(keyboardInput) || controller.ButtonDown(controllerInput);
	case ActionType::hold:
		return keyboard.ButtonHold(keyboardInput) || controller.ButtonHold(controllerInput);
	case ActionType::up:
		return keyboard.ButtonUp(keyboardInput) || controller.ButtonUp(controllerInput);
	}
	return false;
}