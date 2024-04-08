#pragma once
#include <vector>

#include "Structs.h"

namespace mk
{
	class VertexBuffer final
	{
		using Vertices = std::vector<Vertex>;
	public:
		VertexBuffer()	= default;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer& other)					= default;
		VertexBuffer(VertexBuffer&& other) noexcept				= default;
		VertexBuffer& operator=(const VertexBuffer& other)		= default;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept	= default;

		void Draw(VkCommandBuffer commandBuffer) const;
		void SetVertices(const Vertices& vertices);
		void Destroy();

	private:
		void CreateVertexBuffer(const Vertices& vertices);

		VkBuffer m_VertexBuffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_VertexBufferMemory{ VK_NULL_HANDLE };
		uint32_t m_NrVertices{};
		bool m_Destroyed{ true };
	};
}