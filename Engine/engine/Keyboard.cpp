#include "Keyboard.h"

using namespace mk;

Keyboard::Keyboard(GLFWwindow* windowPtr)
	: m_WindowPtr{ windowPtr }
{
}

bool Keyboard::ButtonPress(int scanCode) const
{
	return glfwGetKey(m_WindowPtr, scanCode);
}
