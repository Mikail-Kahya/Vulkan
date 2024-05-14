#include "Controller.h"

#include "Command.h"

using namespace mk;

Controller::Controller(uint8_t idx, KeyInput nrKeys, bool useKeyboard)
	: m_Controller{ idx }
	, m_Keyboard{ useKeyboard ? nrKeys : 0 }
	, m_Idx{ idx }
	, m_UseKeyboard{ useKeyboard }
	, m_Enabled{ true }
{
}

void Controller::KeyDown(KeyInput input)
{
	if (m_UseKeyboard)
		m_Keyboard.PollButton(input, true);
}

void Controller::KeyUp(KeyInput input)
{
	if (m_UseKeyboard)
		m_Keyboard.PollButton(input, false);
}

void Controller::HandleInput()
{
	m_Controller.UpdateInput();
	for (const auto& mapping : m_InputMapping.GetMappings())
	{
		if (mapping.first.Triggered(m_Controller, m_Keyboard))
			mapping.second->Execute();
	}
}

uint8_t Controller::GetIdx() const
{
	return m_Idx;
}

const InputMapping& Controller::GetInputMapping() const
{
	return m_InputMapping;
}

void Controller::AddBinding(const Action& action, Command* commandPtr)
{
	m_InputMapping.AddMapping(action, commandPtr);
}

void Controller::RemoveBinding(Command* commandPtr)
{
	m_InputMapping.RemoveMapping(commandPtr);
}

void Controller::ClearBindings()
{
	m_InputMapping = {};
}

void Controller::Enable()
{
	m_Enabled = true;
}

void Controller::Disable()
{
	m_Enabled = false;
}

bool Controller::IsEnabled() const
{
	return m_Enabled;
}
