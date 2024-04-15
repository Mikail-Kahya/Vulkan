#include "Camera.h"

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

void Camera::Update()
{
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
	//m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	m_ViewMatrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::FlagView()
{
	m_FlagView = true;
}

void Camera::FlagProjection()
{
	m_FlagProjection = true;
}
