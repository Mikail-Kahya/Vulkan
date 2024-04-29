#include "VertexBuffer.h"

#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

VertexBuffer::~VertexBuffer()
{
	DestroyIndices();
	DestroyVertices();
}

void VertexBuffer::Draw(VkCommandBuffer commandBuffer) const
{
	if (!IsValid())
		return;

	const VkBuffer vertexBuffers[] = { m_VertexBuffer };
	constexpr VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdDrawIndexed(commandBuffer, m_NrIndices, 1, 0, 0, 0);
}

void VertexBuffer::SetIndices(const Indices& indices)
{
	DestroyIndices();

	if (!indices.empty())
	{
		m_NrIndices = static_cast<uint32_t>(indices.size());
		CreateIndexBuffer(indices);
	}
}

void VertexBuffer::CreateIndexBuffer(const Indices& indices)
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };
	VkPhysicalDevice physicalDevice{ vulkanBase.GetPhysicalDevice() };

	const VkDeviceSize bufferSize{ sizeof(indices[0]) * indices.size() };

	VkBuffer stagingBuffer{ VK_NULL_HANDLE };
	VkDeviceMemory stagingBufferMemory{ VK_NULL_HANDLE };
	CreateBuffer(	device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data{ nullptr };
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(	device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

	vulkanBase.GetCommandPool().CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VertexBuffer::DestroyVertices()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	if (m_VertexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device, m_VertexBuffer, nullptr);
		m_VertexBuffer = VK_NULL_HANDLE;
	}

	if (m_VertexBufferMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(device, m_VertexBufferMemory, nullptr);
		m_VertexBufferMemory = VK_NULL_HANDLE;
	}
}

void VertexBuffer::DestroyIndices()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	if (m_IndexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device, m_IndexBuffer, nullptr);
		m_IndexBuffer = VK_NULL_HANDLE;
	}
		
	if (m_IndexBufferMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(device, m_IndexBufferMemory, nullptr);
		m_IndexBufferMemory = VK_NULL_HANDLE;
	}
}

bool VertexBuffer::IsValid() const
{
	return	m_NrIndices != 0 &&
			m_VertexBuffer != VK_NULL_HANDLE &&
			m_IndexBuffer != VK_NULL_HANDLE;
}
