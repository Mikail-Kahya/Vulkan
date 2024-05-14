#include "ControllerInput.h"

#include "InputKeys.h"

using namespace mk;

#pragma region Impl
#define WIN32_LEAN_AND_MEAN
#include <functional>
#include <cmath>
#include <windows.h>
#include <Xinput.h>

#include "glm/vec2.hpp"

class ControllerInput::XControllerInput final
{
public:
	XControllerInput(uint8_t idx);
	~XControllerInput();

	XControllerInput(const XControllerInput& other) = delete;
	XControllerInput(XControllerInput&& other) noexcept = delete;
	XControllerInput& operator=(const XControllerInput& other) = delete;
	XControllerInput& operator=(XControllerInput&& other) noexcept = delete;

	void UpdateInput();
	// Deadzone is in a range between 0 and 1
	void SetDeadzone(float deadzone);

	glm::vec2 GetLeftStickInput() const noexcept;
	glm::vec2 GetRightStickInput() const noexcept;
	bool ButtonDown(Input input) const noexcept;
	bool ButtonHold(Input input) const noexcept;
	bool ButtonUp(Input input) const noexcept;

private:
	glm::vec2 GetStickInput(SHORT xInput, SHORT yInput) const noexcept;

	bool XHandleInput(const std::function<bool(int)>& func, Input input) const noexcept;
	bool XButtonDown(int input) const noexcept;
	bool XButtonHold(int input) const noexcept;
	bool XButtonUp(int input) const noexcept;

	static constexpr int MAX_STICK_INPUT{ 32767 };
	float m_Deadzone{};

	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};
	uint8_t m_ControllerIdx{};
};

ControllerInput::XControllerInput::XControllerInput(uint8_t idx)
	: m_ControllerIdx(idx)
{
	constexpr float defaultDeadZone{ 0.05f };
	SetDeadzone(defaultDeadZone);
}

ControllerInput::XControllerInput::~XControllerInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
}

void ControllerInput::XControllerInput::UpdateInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIdx, &m_CurrentState);
}

void ControllerInput::XControllerInput::SetDeadzone(float deadzone)
{
	m_Deadzone = deadzone * MAX_STICK_INPUT;
}

glm::vec2 ControllerInput::XControllerInput::GetLeftStickInput() const noexcept
{
	return GetStickInput(m_CurrentState.Gamepad.sThumbLX, m_CurrentState.Gamepad.sThumbLY);
}

glm::vec2 ControllerInput::XControllerInput::GetRightStickInput() const noexcept
{
	return GetStickInput(m_CurrentState.Gamepad.sThumbRX, m_CurrentState.Gamepad.sThumbRY);
}

bool ControllerInput::XControllerInput::ButtonDown(Input input) const noexcept
{
	return XHandleInput([this](int input)
		{
			return XButtonDown(input);
		}, input);
}

bool ControllerInput::XControllerInput::ButtonHold(Input input) const noexcept
{
	return XHandleInput([this](int input)
		{
			return XButtonHold(input);
		}, input);
}

bool ControllerInput::XControllerInput::ButtonUp(Input input) const noexcept
{
	return XHandleInput([this](int xInput)
		{
			return XButtonUp(xInput);
		}, input);
}

glm::vec2 ControllerInput::XControllerInput::GetStickInput(SHORT xInput, SHORT yInput) const noexcept
{
	float x{ static_cast<float>(xInput) };
	if (abs(x) < m_Deadzone)
		x = 0;

	float y{ static_cast<float>(yInput) };
	if (abs(y) < m_Deadzone)
		y = 0;

	return { x / MAX_STICK_INPUT, y / MAX_STICK_INPUT };
}

bool ControllerInput::XControllerInput::XHandleInput(const std::function<bool(int)>& press, Input input) const noexcept
{
	switch (input) {
	case Input::a:
		return press(XINPUT_GAMEPAD_A);
	case Input::b:
		return press(XINPUT_GAMEPAD_B);
	case Input::y:
		return press(XINPUT_GAMEPAD_Y);
	case Input::x:
		return press(XINPUT_GAMEPAD_X);
	case Input::dPadLeft:
		return press(XINPUT_GAMEPAD_DPAD_LEFT);
	case Input::dPadRight:
		return press(XINPUT_GAMEPAD_DPAD_RIGHT);
	case Input::dPadDown:
		return press(XINPUT_GAMEPAD_DPAD_DOWN);
	case Input::dPadUp:
		return press(XINPUT_GAMEPAD_DPAD_UP);
	case Input::bumperLeft:
		return press(XINPUT_GAMEPAD_LEFT_SHOULDER);
	case Input::bumperRight:
		return press(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	}
	return false;
}

bool ControllerInput::XControllerInput::XButtonDown(int input) const noexcept
{
	return !(m_PreviousState.Gamepad.wButtons & input) *
		(m_CurrentState.Gamepad.wButtons & input);
}

bool ControllerInput::XControllerInput::XButtonHold(int input) const noexcept
{
	return m_CurrentState.Gamepad.wButtons & input;
}

bool ControllerInput::XControllerInput::XButtonUp(int input) const noexcept
{
	return	 (m_PreviousState.Gamepad.wButtons & input) *
		!(m_CurrentState.Gamepad.wButtons & input);
}
#pragma endregion Impl

ControllerInput::ControllerInput(uint8_t idx)
	: m_XInputImpl{ new XControllerInput(idx) }
{
}

ControllerInput::~ControllerInput()
{
	delete m_XInputImpl;
}

ControllerInput::ControllerInput(ControllerInput&& other) noexcept
	: m_XInputImpl{ other.m_XInputImpl }
{
	other.m_XInputImpl = nullptr;
}

ControllerInput& ControllerInput::operator=(ControllerInput&& other) noexcept
{
	m_XInputImpl = other.m_XInputImpl;
	other.m_XInputImpl = nullptr;

	return *this;
}

void ControllerInput::UpdateInput()
{
	m_XInputImpl->UpdateInput();
}

void ControllerInput::SetDeadzone(float deadzone)
{
	m_XInputImpl->SetDeadzone(deadzone);
}

glm::vec2 ControllerInput::GetLeftStickInput() const noexcept
{
	return m_XInputImpl->GetLeftStickInput();
}

glm::vec2 ControllerInput::GetRightStickInput() const noexcept
{
	return m_XInputImpl->GetRightStickInput();
}

glm::vec2 ControllerInput::GetDPadInput() const noexcept
{
	glm::vec2 input{};
	if (ButtonHold(Input::dPadLeft))
		--input.x;

	if (ButtonHold(Input::dPadRight))
		++input.x;

	if (ButtonHold(Input::dPadDown))
		--input.y;

	if (ButtonHold(Input::dPadUp))
		++input.y;

	return input;
}

bool ControllerInput::ButtonDown(Input input) const noexcept
{
	return m_XInputImpl->ButtonDown(input);
}

bool ControllerInput::ButtonHold(Input input) const noexcept
{
	return m_XInputImpl->ButtonHold(input);
}

bool ControllerInput::ButtonUp(Input input) const noexcept
{
	return m_XInputImpl->ButtonUp(input);
}