#pragma once
#include <vector>

#include "Structs.h"

namespace mk
{
	class VertexBuffer final
	{
		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<uint16_t>;
	public:
		VertexBuffer()	= default;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer& other)					= default;
		VertexBuffer(VertexBuffer&& other) noexcept				= default;
		VertexBuffer& operator=(const VertexBuffer& other)		= default;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept	= default;

		void Draw(VkCommandBuffer commandBuffer) const;
		void SetVertices(const Vertices& vertices);
		void SetIndices(const Indices& indices);
		void Destroy();

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