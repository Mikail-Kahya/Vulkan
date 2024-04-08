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
	const VkDevice& device{ VulkanBase::GetInstance().GetDevice() };
	vkDestroyBuffer(device, m_VertexBuffer, nullptr);
	vkFreeMemory(device, m_VertexBufferMemory, nullptr);
}

void VertexBuffer::CreateVertexBuffer(const Vertices& vertices)
{
	m_Destroyed = false;

	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	const VkDevice& device{ vulkanBase.GetDevice() };

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_VertexBuffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to create vertex buffer");

	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(device, m_VertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(vulkanBase.GetPhysicalDevice(), memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate vertex buffer memory");

	vkBindBufferMemory(device, m_VertexBuffer, m_VertexBufferMemory, 0);

	// Fill in vertices info
	void* data;
	vkMapMemory(device, m_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(device, m_VertexBufferMemory);
}
