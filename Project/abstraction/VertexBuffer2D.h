#pragma once
#include <vector>

#include "engine/Structs.h"

namespace mk
{
	class VertexBuffer2D final
	{
		using Vertices = std::vector<Vertex2D>;
		using Indices = std::vector<uint16_t>;
	public:
		VertexBuffer2D()	= default;
		~VertexBuffer2D();

		VertexBuffer2D(const VertexBuffer2D& other)					= default;
		VertexBuffer2D(VertexBuffer2D&& other) noexcept				= default;
		VertexBuffer2D& operator=(const VertexBuffer2D& other)		= default;
		VertexBuffer2D& operator=(VertexBuffer2D&& other) noexcept	= default;

		void Draw(VkCommandBuffer commandBuffer) const;
		void SetVertices(const Vertices& vertices);
		void SetIndices(const Indices& indices);

	private:
		void CreateVertexBuffer(const Vertices& vertices);
		void CreateIndexBuffer(const Indices& indices);
		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		void DestroyVertices();
		void DestroyIndices();
		bool IsValid() const;

		VkBuffer m_VertexBuffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_VertexBufferMemory{ VK_NULL_HANDLE };
		VkBuffer m_IndexBuffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_IndexBufferMemory{ VK_NULL_HANDLE };

		uint32_t m_NrIndices{};
	};
}