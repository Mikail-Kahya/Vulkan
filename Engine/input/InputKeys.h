#pragma once
#include <cstdint>

enum class Input : uint8_t
{
	a,
	b,
	y,
	x,
	dPadLeft,
	dPadRight,
	dPadDown,
	dPadUp,
	stickLeft,
	stickRight,
	triggerLeft,
	triggerRight,
	bumperLeft,
	bumperRight
};


enum class ActionType : uint8_t
{
	hold,
	down,
	up
};