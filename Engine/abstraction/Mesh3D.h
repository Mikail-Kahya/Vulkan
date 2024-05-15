#pragma once
#include <string>
#include <vector>

#include "core/Structs.h"
#include "abstraction/UniformBuffer.h"
#include "abstraction/VertexBuffer.h"

namespace mk
{
	class Mesh3D final
	{
		using Vertex = Vertex3D;
	public:
		Mesh3D() = default;
		~Mesh3D()	= default;

		Mesh3D(const Mesh3D& other)					= delete;
		Mesh3D(Mesh3D&& other) noexcept				= delete;
		Mesh3D& operator=(const Mesh3D& other)		= delete;
		Mesh3D& operator=(Mesh3D&& other) noexcept	= delete;

		void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const;
		void Update();
		void Load(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		void Load(const std::string& objFile);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);

		void SetTexture(const std::string& textureFile);

	private:
		void ClampRotation();
		float ClampAxis(float angle) const;
		void SetTransform();
		void FlagTransform();

		VertexBuffer m_VertexBuffer{};
		std::unique_ptr<UniformBuffer> m_UniformBuffer{};
		glm::mat4 m_WorldTransform{};
		glm::vec3 m_Position{};
		glm::vec3 m_Rotation{};
		glm::vec3 m_Scale{1, 1, 1};

		bool m_FlagTransform{ true };
	};
}
