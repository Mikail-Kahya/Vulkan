#include "Mouse.h"

#include <iostream>

#include "glm/geometric.hpp"

using namespace mk;

Mouse::Mouse(GLFWwindow* windowPtr)
	: m_WindowPtr{ windowPtr }
{
}

void Mouse::Update()
{
	SetViewport();
	const glm::vec2 newPos{ GetWindowPos() };
	m_Displacement = newPos - m_Position;
	const float distSqr{ m_Displacement.x * m_Displacement.x + m_Displacement.y * m_Displacement.y };
	m_Direction = (distSqr > FLT_EPSILON) ? glm::normalize(m_Displacement) : glm::vec2{};
	m_Position = newPos;
}

const glm::vec2& Mouse::GetDirection() const
{
	return m_Direction;
}

const glm::vec2& Mouse::GetDisplacement() const
{
	return m_Displacement;
}

glm::vec2 Mouse::GeScreenPos() const
{
	return m_Position;
}

bool Mouse::LeftClick() const
{
	return glfwGetMouseButton(m_WindowPtr, GLFW_MOUSE_BUTTON_LEFT);
}

bool Mouse::RightClick() const
{
	return glfwGetMouseButton(m_WindowPtr, GLFW_MOUSE_BUTTON_RIGHT);
}

glm::vec2 Mouse::GetWindowPos() const
{
	double x{};
	double y{};
	glfwGetCursorPos(m_WindowPtr, &x, &y);

	int height{};
	glfwGetWindowSize(m_WindowPtr, nullptr, &height);
	y = height - y;

	return { static_cast<float>(x), static_cast<float>(y) };
}

void Mouse::SetViewport()
{
	int width{};
	int height{};
	glfwGetWindowSize(m_WindowPtr, &width, &height);
	m_Center = { static_cast<float>(width) * 0.5f,  static_cast<float>(height) * 0.5f };
}