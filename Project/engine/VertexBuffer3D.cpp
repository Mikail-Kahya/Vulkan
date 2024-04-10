#include "VertexBuffer3D.h"

#include <stdexcept>

#include "vulkanbase/VulkanBase.h"
#include "vulkanbase/VulkanUtils.h"

using namespace mk;

VertexBuffer3D::~VertexBuffer3D()
{
	Destroy();
}

void VertexBuffer3D::Draw(VkCommandBuffer commandBuffer) const
{
	if (!IsValid())
		return;

	const VkBuffer vertexBuffers[] = { m_VertexBuffer };
	constexpr VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdDrawIndexed(commandBuffer, m_NrIndices, 1, 0, 0, 0);
}

void VertexBuffer3D::SetVertices(const Vertices& vertices)
{
	DestroyVertices();

	if (!vertices.empty())
		CreateVertexBuffer(vertices);
}

void VertexBuffer3D::SetIndices(const Indices& indices)
{
	DestroyIndices();

	if (!indices.empty())
	{
		m_NrIndices = static_cast<uint32_t>(indices.size());
		CreateIndexBuffer(indices);
	}
		
}

void VertexBuffer3D::Destroy()
{
	DestroyIndices();
	DestroyVertices();
}

void VertexBuffer3D::CreateVertexBuffer(const Vertices& vertices)
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };

	const VkDeviceSize bufferSize{ sizeof(vertices[0]) * vertices.size() };

	VkBuffer stagingBuffer{ VK_NULL_HANDLE };
	VkDeviceMemory stagingBufferMemory{ VK_NULL_HANDLE };
	CreateBuffer(	bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					stagingBuffer, stagingBufferMemory);

	// Fill in vertices info
	void* data{ nullptr };
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(	bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

	vulkanBase.GetCommandPool().CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VertexBuffer3D::CreateIndexBuffer(const Indices& indices)
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };

	const VkDeviceSize bufferSize{ sizeof(indices[0]) * indices.size() };

	VkBuffer stagingBuffer{ VK_NULL_HANDLE };
	VkDeviceMemory stagingBufferMemory{ VK_NULL_HANDLE };
	CreateBuffer(	bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data{ nullptr };
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(	bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

	vulkanBase.GetCommandPool().CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VertexBuffer3D::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };

	VertexBuffer3D vertexBuffer{};

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

void VertexBuffer3D::DestroyVertices()
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

void VertexBuffer3D::DestroyIndices()
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

bool VertexBuffer3D::IsValid() const
{
	return	m_NrIndices != 0 &&
			m_VertexBuffer != VK_NULL_HANDLE &&
			m_IndexBuffer != VK_NULL_HANDLE;
}
