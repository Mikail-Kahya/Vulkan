#pragma once

#include <string>
#include "GLFW/glfw3.h"

namespace mk
{
	// Not to be confused with windows
	class Window final
	{
	public:
		Window(int width, int height, std::string name);
		~Window();

		Window(const Window& other)					= default;
		Window(Window&& other) noexcept				= default;
		Window& operator=(const Window& other)		= default;
		Window& operator=(Window&& other) noexcept	= default;

		GLFWwindow* GetWindow() const;
		bool ShouldClose() const;

	private:

		GLFWwindow* m_WindowPtr{ nullptr };
	};
}