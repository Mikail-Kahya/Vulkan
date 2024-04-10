#pragma once
#include <vector>

#include "Structs.h"

namespace mk
{
	class VertexBuffer3D final
	{
		using Vertices = std::vector<Vertex3D>;
		using Indices = std::vector<uint16_t>;
	public:
		VertexBuffer3D()	= default;
		~VertexBuffer3D();

		VertexBuffer3D(const VertexBuffer3D& other)					= default;
		VertexBuffer3D(VertexBuffer3D&& other) noexcept				= default;
		VertexBuffer3D& operator=(const VertexBuffer3D& other)		= default;
		VertexBuffer3D& operator=(VertexBuffer3D&& other) noexcept	= default;

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