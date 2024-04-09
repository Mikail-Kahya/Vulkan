#include "VertexBuffer.h"

#include <stdexcept>

#include "vulkanbase/VulkanBase.h"
#include "vulkanbase/VulkanUtils.h"

using namespace mk;

VertexBuffer::~VertexBuffer()
{
	if (!m_Destroyed)
		Destroy();
}

void VertexBuffer::Draw(VkCommandBuffer commandBuffer) const
{
	if (m_NrVertices == 0)
		return;

	const VkBuffer vertexBuffers[] = { m_VertexBuffer };
	constexpr VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdDraw(commandBuffer, m_NrVertices, 1, 0, 0);
}

void VertexBuffer::SetVertices(const Vertices& vertices)
{
	if (!m_Destroyed)
		Destroy();

	if (!vertices.empty())
	{
		m_NrVertices = static_cast<uint32_t>(vertices.size());
		CreateVertexBuffer(vertices);
	}
}

void VertexBuffer::Destroy()
{
	m_Destroyed = true;
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };
	vkDestroyBuffer(device, m_VertexBuffer, nullptr);
	vkFreeMemory(device, m_VertexBufferMemory, nullptr);
}

void VertexBuffer::CreateVertexBuffer(const Vertices& vertices)
{
	m_Destroyed = false;
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };

	VkDeviceSize bufferSize{ sizeof(vertices[0]) * vertices.size() };

	VkBuffer stagingBuffer{ VK_NULL_HANDLE };
	VkDeviceMemory stagingBufferMemory{ VK_NULL_HANDLE };
	CreateBuffer(	bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					stagingBuffer, stagingBufferMemory);

	// Fill in vertices info
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(	bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

	vulkanBase.GetCommandPool().CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VertexBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
	VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };

	VertexBuffer vertexBuffer{};

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to create buffer");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(vulkanBase.GetPhysicalDevice(), memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate buffer memory");

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}