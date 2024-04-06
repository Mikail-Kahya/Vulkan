#include "CommandPool.h"
#include "CommandPool.h"

#include <stdexcept>

#include "vulkanbase/VulkanBase.h"
#include "vulkanbase/VulkanStructs.h"
#include "vulkanbase/VulkanUtils.h"

using namespace mk;

CommandPool::~CommandPool()
{
	if (!m_Destroy)
		Destroy();
}

void CommandPool::Initialize()
{
	m_Destroy = false;

	CreateCommandPool();
}

void CommandPool::Destroy()
{
	m_Destroy = true;

	const VkDevice& device{ VulkanBase::GetInstance().GetDevice() };
	vkDestroyCommandPool(device, m_CommandPool, nullptr);
}

std::vector<VkCommandBuffer> CommandPool::CreateCommandBuffer(int nrBuffers) const
{
	std::vector<VkCommandBuffer> commandBuffers(nrBuffers);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = nrBuffers;

	if (vkAllocateCommandBuffers(VulkanBase::GetInstance().GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate command buffers");

	return commandBuffers;
}

void CommandPool::CreateCommandPool()
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	QueueFamilyIndices queueFamilyIndices{ FindQueueFamilies(vulkanBase.GetPhysicalDevice(), vulkanBase.GetSurface()) };

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(vulkanBase.GetDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		throw std::runtime_error("Failed to create command pool");
}
