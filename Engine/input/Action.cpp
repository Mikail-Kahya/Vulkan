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

glm::vec2 DirectionAction::Triggered(const ControllerInput& controller, const Mouse* mouse) const
{
    switch (inputType)
    {
    case InputType::leftStick:
        return controller.GetLeftStickInput();
    case InputType::mouse:
        return mouse->GetDisplacement();
	case InputType::rightStick:
		return controller.GetRightStickInput();
    }
    return {};
}
