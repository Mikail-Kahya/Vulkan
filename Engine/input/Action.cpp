#include "Action.h"

using namespace mk;

void Action::SetControllerInput(Input button)
{
	m_ControllerInput = button;
}

void Action::SetKeyboardInput(int32_t button)
{
	m_KeyboardInput = button;
}

void Action::SetType(ActionType inputType)
{
	m_Type = inputType;
}

bool Action::Triggered(const ControllerInput& controller, const KeyboardInput& keyboard) const
{
	switch (m_Type)
	{
	case ActionType::down:
		return keyboard.ButtonDown(m_KeyboardInput) || controller.ButtonDown(m_ControllerInput);
	case ActionType::hold:
		return keyboard.ButtonHold(m_KeyboardInput) || controller.ButtonHold(m_ControllerInput);
	case ActionType::up:
		return keyboard.ButtonUp(m_KeyboardInput) || controller.ButtonUp(m_ControllerInput);
	}
	return false;
}

KeyInput Action::GetKeyInput() const
{
	return m_KeyboardInput;
}
