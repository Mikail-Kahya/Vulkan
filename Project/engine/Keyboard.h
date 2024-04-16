#pragma once

#include "GLFW/glfw3.h"

namespace mk
{
	class Keyboard final
	{
	public:
		Keyboard(GLFWwindow* windowPtr);
		~Keyboard() = default;

		Keyboard(const Keyboard& other)					= default;
		Keyboard(Keyboard&& other) noexcept				= default;
		Keyboard& operator=(const Keyboard& other)		= default;
		Keyboard& operator=(Keyboard&& other) noexcept	= default;

		bool ButtonPress(int scanCode) const;

	private:
		GLFWwindow* m_WindowPtr{ nullptr };
	};
}
