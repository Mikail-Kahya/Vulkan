#pragma once
#include <vector>

#include "Structs.h"
#include "UniformBuffer.h"
#include "VertexBuffer3D.h"

namespace mk
{
	class Mesh3D final
	{
		using VertexBuffer = VertexBuffer3D;
		using Vertex = Vertex3D;
	public:
		Mesh3D()	= default;
		~Mesh3D()	= default;

		Mesh3D(const Mesh3D& other)					= delete;
		Mesh3D(Mesh3D&& other) noexcept				= delete;
		Mesh3D& operator=(const Mesh3D& other)		= delete;
		Mesh3D& operator=(Mesh3D&& other) noexcept	= delete;

		void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const;
		void Update();
		void Load(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);

		void AddPosition(const glm::vec3& position);
		void AddRotation(const glm::vec3& rotation);
		void AddScale(const glm::vec3& scale);

	private:
		void ClampRotation();
		float ClampAxis(float angle) const;
		void SetTransform();
		void FlagTransform();

		UniformBuffer m_UniformBuffer{};
		VertexBuffer m_VertexBuffer{};
		glm::mat4 m_WorldTransform{};
		glm::vec3 m_Position{};
		glm::vec3 m_Rotation{};
		glm::vec3 m_Scale{1, 1, 1};

		bool m_FlagTransform{ true };
	};
}
