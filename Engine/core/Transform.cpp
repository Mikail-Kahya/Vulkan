#include "Transform.h"
#include <cmath>

using namespace mk;


const glm::vec3& Transform::GetPosition() const
{
	return m_Position;
}

const glm::vec3& Transform::GetRotation() const
{
	return m_Rotation;
}

const glm::vec3& Transform::GetScale() const
{
	return m_Scale;
}

void Transform::AddOffset(const glm::vec3& offset)
{
	m_Position += offset;
}

void Transform::AddRotation(const glm::vec3& deltaRotation)
{
	m_Rotation += deltaRotation;
}

void Transform::AddScale(const glm::vec3& deltaScale)
{
	m_Scale += deltaScale;
}

void Transform::AddPitch(float deltaPitch)
{
	m_Rotation.x = OverflowAngle(m_Rotation.x + deltaPitch);
}

void Transform::AddYaw(float deltaYaw)
{
	m_Rotation.y = OverflowAngle(m_Rotation.y + deltaYaw);
}

void Transform::AddRoll(float deltaRoll)
{
	m_Rotation.z = OverflowAngle(m_Rotation.z + deltaRoll);
}

void Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void Transform::SetRotation(const glm::vec3& rotation)
{
	m_Rotation.x = OverflowAngle(rotation.x);
	m_Rotation.y = OverflowAngle(rotation.y);
	m_Rotation.z = OverflowAngle(rotation.z);
}

void Transform::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

float Transform::OverflowAngle(float angle)
{
	return std::fmod(angle, 360.f) - 180.f;
}
