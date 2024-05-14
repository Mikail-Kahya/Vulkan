#include "Camera.h"

#include <algorithm>

#include "Time.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

using namespace mk;

Camera::Camera()
{
	SetViewport(1280, 720);
	SetFOV(80);
}

Camera::Camera(float width, float height, float fovDeg)
{
	SetViewport(width, height);
	SetFOV(fovDeg);
}

void Camera::Update(const Mouse& mouse, const Keyboard& keyboard)
{
	if (mouse.RightClick())
		Rotate(mouse.GetDisplacement());

	Move(keyboard);

	if (m_FlagProjection)
		CalculateProjectionMatrix();

	if (m_FlagView)
		CalculateViewMatrix();
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void Camera::SetViewport(float width, float height)
{
	FlagProjection();
	m_AspectRatio = width / height;
}

void Camera::SetFOV(float fovDeg)
{
	FlagProjection();
	m_FOV = glm::radians(fovDeg);
}

void Camera::CalculateProjectionMatrix()
{
	m_FlagProjection = false;
	m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_ZNear, m_ZFar);
	m_ProjectionMatrix[1][1] *= -1;
}

void Camera::CalculateViewMatrix()
{
	m_FlagView = false;
	// Right handed
	m_Right = glm::normalize(glm::cross(m_Forward, { 0,1,0 }));
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
	
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

void Camera::Move(const Keyboard& keyboard)
{
	bool hasMoved{ false };
	glm::vec2 moveDir{};
	
	if (keyboard.ButtonPress(GLFW_KEY_W))
	{
		moveDir.y += 1;
		hasMoved = true;
	}
		
	if (keyboard.ButtonPress(GLFW_KEY_S))
	{
		moveDir.y -= 1;
		hasMoved = true;
	}
		
	if (keyboard.ButtonPress(GLFW_KEY_D))
	{
		moveDir.x += 1;
		hasMoved = true;
	}
		
	if (keyboard.ButtonPress(GLFW_KEY_A))
	{
		moveDir.x -= 1;
		hasMoved = true;
	}

	if (!hasMoved)
		return;

	moveDir *= m_MoveSpeed * Time::DeltaTime();
	m_Position += m_Forward * moveDir.y;
	m_Position += m_Right * moveDir.x;
	FlagView();
}

void Camera::Rotate(const glm::vec2& mouseDir)
{
	const float deltaTime{ Time::DeltaTime() };
	bool mouseMoved{ false };
	
	if (abs(mouseDir.x) > FLT_EPSILON)
	{
		m_TotalYaw -= mouseDir.x * m_RotateSpeed * deltaTime;
		m_TotalYaw = std::fmod(m_TotalYaw, 360.f);
		mouseMoved = true;
	}
	if (abs(mouseDir.y) > FLT_EPSILON)
	{
		// Flor allowed this :)
		m_TotalPitch -= mouseDir.y * m_RotateSpeed * deltaTime;
		m_TotalPitch = std::clamp(m_TotalPitch, -90.f, 90.f);
		mouseMoved = true;
	}

	if (!mouseMoved)
		return;
	FlagView();
	//glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_TotalPitch), glm::vec3{ 1, 0, 0 });
	//rotation = glm::rotate(rotation, glm::radians(m_TotalYaw), glm::vec3{ 0, 1, 0 });

	glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_TotalYaw), glm::vec3{ 0, 1, 0 });
	rotation = glm::rotate(rotation, glm::radians(m_TotalPitch), glm::vec3{ 1, 0, 0 });

	m_Forward = glm::normalize(rotation * glm::vec4{ 0, 0, 1, 1 });
}

void Camera::FlagView()
{
	m_FlagView = true;
}

void Camera::FlagProjection()
{
	m_FlagProjection = true;
}
