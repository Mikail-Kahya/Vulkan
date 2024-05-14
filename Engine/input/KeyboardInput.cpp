#include "KeyboardInput.h"

#include <algorithm>

using namespace mk;

KeyboardInput::KeyboardInput(KeyInput nrKeys)
{
	m_PrevKeys.resize(nrKeys);
	m_CurrentKeys.resize(nrKeys);
}

void KeyboardInput::Update()
{
	std::copy(m_CurrentKeys.begin(), m_CurrentKeys.end(), m_PrevKeys.begin());
}

void KeyboardInput::PollButton(KeyInput button, bool isPressed)
{
	m_CurrentKeys[button] = isPressed;
}

bool KeyboardInput::ButtonDown(KeyInput key) const
{
	return !m_PrevKeys[key] && m_CurrentKeys[key];
}

bool KeyboardInput::ButtonHold(KeyInput key) const
{
	return m_CurrentKeys[key];}

bool KeyboardInput::ButtonUp(KeyInput key) const
{
	return m_PrevKeys[key] && !m_CurrentKeys[key];
}