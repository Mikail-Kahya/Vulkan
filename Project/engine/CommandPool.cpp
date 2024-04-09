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

	VkDevice device{ VulkanBase::GetInstance().GetDevice() };
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

void CommandPool::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };
	VkQueue graphicsQueue{ vulkanBase.GetGraphicsQueue() };

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(VulkanBase::GetInstance().GetDevice(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, m_CommandPool, 1, &commandBuffer);
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
