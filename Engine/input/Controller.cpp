#include "Controller.h"

#include "Command.h"
#include "Core/ServiceLocator.h"

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
        m_Keyboard.PollButton(input, false);
}

void Controller::KeyUp(KeyInput input)
{
	if (m_UseKeyboard)
        m_Keyboard.PollButton(input, true);
}

void Controller::HandleInput()
{
	m_Controller.UpdateInput();
	for (const auto& mapping : m_InputMapping.GetMappings())
	{
		if (mapping.first.Triggered(m_Controller, m_Keyboard))
			mapping.second->Execute();
	}
	const Mouse* mousePtr{ ServiceLocator::GetInputSystem().GetMouse() };
	for (const auto& mapping : m_DirectionInputMapping.GetMappings())
	{
        constexpr float epsilon{ 0.0001f };
        const glm::vec2 direction{ mapping.first.Triggered(m_Controller, mousePtr) };
        if (abs(direction.x) > epsilon || abs(direction.y) > epsilon)
            mapping.second->Execute(direction);
	}
}

uint8_t Controller::GetIdx() const
{
	return m_Idx;
}

const Controller::DefaultInputMapping& Controller::GetInputMapping() const
{
	return m_InputMapping;
}

const Controller::DirectionInputMapping& Controller::GetDirectionInputMapping() const
{
	return m_DirectionInputMapping;
}

void Controller::AddBinding(const Action& action, Command* commandPtr)
{
	m_InputMapping.AddMapping(action, commandPtr);
}

void Controller::RemoveBinding(Command* commandPtr)
{
	m_InputMapping.RemoveMapping(commandPtr);
}

void Controller::AddDirectionBinding(const DirectionAction& action, DirectionCommand* commandPtr)
{
	m_DirectionInputMapping.AddMapping(action, commandPtr);
}

void Controller::RemoveDirectionBinding(DirectionCommand* commandPtr)
{
	m_DirectionInputMapping.RemoveMapping(commandPtr);
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
