#include "Mesh3D.h"

// temp includes
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"

using namespace mk;

void Mesh3D::Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const
{
	if (!m_VertexBuffer.IsValid())
		return;
	m_UniformBuffer.SetActive(commandBuffer, pipelineLayout);
	m_VertexBuffer.Draw(commandBuffer);
}

void Mesh3D::Update()
{
	// Temp hardcoded rotation
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	SetRotation({ 0.f, 0.f, time * 90.0f});

	if (m_FlagTransform)
		SetTransform();

	m_UniformBuffer.Update(m_WorldTransform);
}

void Mesh3D::Load(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	m_VertexBuffer.SetVertices(vertices);
	m_VertexBuffer.SetIndices(indices);
}

void Mesh3D::Load(const std::string& objFile)
{
	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};
	if (ParseOBJ<Vertex3D>(objFile, vertices, indices))
		Load(vertices, indices);
}

void Mesh3D::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	FlagTransform();
}

void Mesh3D::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = rotation;
	ClampRotation();
	FlagTransform();
}

void Mesh3D::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
	FlagTransform();	
}

void Mesh3D::AddPosition(const glm::vec3& position)
{
	m_Position += position;
	FlagTransform();
}

void Mesh3D::AddRotation(const glm::vec3& rotation)
{
	m_Rotation += rotation;
	ClampRotation();
	FlagTransform();
}

void Mesh3D::AddScale(const glm::vec3& scale)
{
	m_Scale += scale;
	FlagTransform();
}

void Mesh3D::ClampRotation()
{
	m_Rotation.x = ClampAxis(m_Rotation.x);
	m_Rotation.y = ClampAxis(m_Rotation.y);
	m_Rotation.z = ClampAxis(m_Rotation.z);
}

float Mesh3D::ClampAxis(float angle) const
{
	while (angle > 180)
		angle -= 360;
	while (angle < -180)
		angle += 360;

	return angle;
}

void Mesh3D::SetTransform()
{
	m_FlagTransform = false;
	constexpr glm::mat4 identity{ 1.f };

	constexpr glm::vec3 x{ 1, 0, 0 };
	constexpr glm::vec3 y{ 0, 1, 0 };
	constexpr glm::vec3 z{ 0, 0, 1 };

	m_WorldTransform = glm::translate(identity, m_Position);
	m_WorldTransform *= glm::rotate(m_WorldTransform, glm::radians(m_Rotation.z), z);
	m_WorldTransform *= glm::scale(m_WorldTransform, m_Scale);
}

void Mesh3D::FlagTransform()
{
	m_FlagTransform = true;
}
