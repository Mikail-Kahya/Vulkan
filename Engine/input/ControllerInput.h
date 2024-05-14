#pragma once
#include "glm/vec2.hpp"
#include "InputKeys.h"

namespace mk
{
	class ControllerInput final
	{
	public:
		ControllerInput(uint8_t idx);
		~ControllerInput();

		ControllerInput(const ControllerInput& other)					= delete;
		ControllerInput(ControllerInput&& other) noexcept;
		ControllerInput& operator=(const ControllerInput& other)		= delete;
		ControllerInput& operator=(ControllerInput&& other) noexcept;

		void UpdateInput();
		// Deadzone is in a range between 0 and 1
		void SetDeadzone(float deadzone);

		glm::vec2 GetLeftStickInput() const noexcept;
		glm::vec2 GetRightStickInput() const noexcept;
		glm::vec2 GetDPadInput() const noexcept;

		bool ButtonDown(Input input) const noexcept;
		bool ButtonHold(Input input) const noexcept;
		bool ButtonUp(Input input) const noexcept;

	private:
		class XControllerInput;

		XControllerInput* m_XInputImpl{};
	};
}
