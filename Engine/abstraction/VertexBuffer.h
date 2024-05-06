#pragma once
#include <vector>

#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanUtils.h"

namespace mk
{
	class VertexBuffer final
	{
		using Indices = std::vector<uint32_t>;
	public:
		VertexBuffer()	= default;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer& other)					= default;
		VertexBuffer(VertexBuffer&& other) noexcept				= default;
		VertexBuffer& operator=(const VertexBuffer& other)		= default;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept	= default;

		void Draw(VkCommandBuffer commandBuffer) const;
		template< typename Vertex>
		void SetVertices(const std::vector<Vertex>& vertices);
		void SetIndices(const Indices& indices);
		bool IsValid() const;

	private:
		template< typename Vertex>
		void CreateVertexBuffer(const std::vector<Vertex>& vertices);
		void CreateIndexBuffer(const Indices& indices);

		void DestroyVertices();
		void DestroyIndices();
		

		VkBuffer m_VertexBuffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_VertexBufferMemory{ VK_NULL_HANDLE };
		VkBuffer m_IndexBuffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_IndexBufferMemory{ VK_NULL_HANDLE };

		uint32_t m_NrIndices{};
	};

	template <typename Vertex>
	void VertexBuffer::SetVertices(const std::vector<Vertex>& vertices)
	{
		DestroyVertices();

		if (!vertices.empty())
			CreateVertexBuffer<Vertex>(vertices);
	}

	template <typename Vertex>
	void VertexBuffer::CreateVertexBuffer(const std::vector<Vertex>& vertices)
	{
		const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
		const VkDevice device{ vulkanBase.GetDevice() };
		const VkPhysicalDevice physicalDevice{ vulkanBase.GetPhysicalDevice() };

		const VkDeviceSize bufferSize{ sizeof(vertices[0]) * vertices.size() };

		VkBuffer stagingBuffer{ VK_NULL_HANDLE };
		VkDeviceMemory stagingBufferMemory{ VK_NULL_HANDLE };
		CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		// Fill in vertices info
		void* data{ nullptr };
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(device, stagingBufferMemory);

		CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		vulkanBase.GetCommandPool().CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}
}
