#pragma once
#include "InputKeys.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "Mouse.h"

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

    struct DirectionAction final
    {
        enum class InputType{ mouse, leftStick, rightStick };

        DirectionAction() = default;
        ~DirectionAction() = default;

        DirectionAction(const DirectionAction& other)					= default;
        DirectionAction(DirectionAction&& other) noexcept				= default;
        DirectionAction& operator=(const DirectionAction& other)		= default;
        DirectionAction& operator=(DirectionAction&& other) noexcept	= default;

        glm::vec2 Triggered(const ControllerInput& controller, const Mouse* mouse) const;

        InputType inputType{};
    };
}
