#pragma once
#include "InputKeys.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"

namespace mk
{
	struct Action final
	{
		Action() = default;
		~Action() = default;

		Action(const Action& other)					= default;
		Action(Action&& other) noexcept				= default;
		Action& operator=(const Action& other)		= default;
		Action& operator=(Action&& other) noexcept	= default;

		bool Triggered(const ControllerInput& controller, const KeyboardInput& keyboard) const;

		Input controllerInput{};
		KeyInput keyboardInput{};
		ActionType type{};
	};
}
