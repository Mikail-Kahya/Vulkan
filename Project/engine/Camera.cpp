#include "Camera.h"

#include <algorithm>

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

void Camera::Update(const Mouse& mouse)
{
	if (m_FlagProjection)
		CalculateProjectionMatrix();

	if (m_FlagView)
		CalculateViewMatrix();

	if (mouse.RightClick())
		Rotate(mouse.GetDirection());
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
	m_Position = { 0, -25, 2 };
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	
	//m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::Rotate(const glm::vec2& mouseDir)
{
	bool mouseMoved{ false };
	if (abs(mouseDir.x) > FLT_EPSILON)
	{
		m_TotalYaw += mouseDir.x * m_RotateSpeed;
		m_TotalYaw = std::fmod(m_TotalYaw, 360.f);
		mouseMoved = true;
	}
	if (abs(mouseDir.y) > FLT_EPSILON)
	{
		// Flor allowed this :)
		m_TotalPitch -= mouseDir.y * m_RotateSpeed;
		m_TotalPitch = std::clamp(m_TotalPitch, -90.f, 90.f);
		mouseMoved = true;
	}

	if (mouseMoved)
	{
		FlagView();
		glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_TotalPitch), glm::vec3{ 0, 0, 1 });
		rotation = glm::rotate(rotation, glm::radians(m_TotalYaw), { 0, 1, 0 });
		m_Forward = rotation * glm::vec4{ m_Forward, 1 };
	}
}

void Camera::FlagView()
{
	m_FlagView = true;
}

void Camera::FlagProjection()
{
	m_FlagProjection = true;
}
