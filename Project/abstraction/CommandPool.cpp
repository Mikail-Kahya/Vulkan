#include "CommandPool.h"

#include <stdexcept>

#include "CommandBuffer.h"
#include "SecondaryCommandBuffer.h"
#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanStructs.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

CommandPool::CommandPool()
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

CommandPool::~CommandPool()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	vkDestroyCommandPool(device, m_CommandPool, nullptr);
	m_CommandPool = VK_NULL_HANDLE;
}

std::vector<VkCommandBuffer> CommandPool::CreateCommandBuffer(int nrBuffers, VkCommandBufferLevel bufferType) const
{
	std::vector<VkCommandBuffer> commandBuffers(nrBuffers);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.level = bufferType;
	allocInfo.commandBufferCount = nrBuffers;

	if (vkAllocateCommandBuffers(VulkanBase::GetInstance().GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate command buffers");

	return commandBuffers;
}

void CommandPool::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer);
}

CommandBuffer CommandPool::CreatePrimaryBuffer() const
{
	return CommandBuffer{ CreateCommandBuffer(VulkanBase::MAX_FRAMES_IN_FLIGHT, VK_COMMAND_BUFFER_LEVEL_PRIMARY) };
}

SecondaryCommandBuffer CommandPool::CreateSecondaryBuffer() const
{
	return SecondaryCommandBuffer{ CreateCommandBuffer(VulkanBase::MAX_FRAMES_IN_FLIGHT, VK_COMMAND_BUFFER_LEVEL_SECONDARY) };
}

VkCommandBuffer CommandPool::BeginSingleTimeCommands() const
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer{ VK_NULL_HANDLE };
	vkAllocateCommandBuffers(VulkanBase::GetInstance().GetDevice(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}

void CommandPool::EndSingleTimeCommands(VkCommandBuffer commandBuffer) const
{
	vkEndCommandBuffer(commandBuffer);

	VkQueue graphicsQueue{ VulkanBase::GetInstance().GetGraphicsQueue() };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(VulkanBase::GetInstance().GetDevice(), m_CommandPool, 1, &commandBuffer);
}