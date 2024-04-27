#include "CommandBuffer.h"

#include <stdexcept>

#include "vulkanbase/VulkanBase.h"

using namespace mk;

CommandBuffer::CommandBuffer(std::vector<VkCommandBuffer>&& commandBuffers)
	: m_CommandBuffers{ std::move(commandBuffers) }
{
}

CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
	: m_CommandBuffers{ std::move(other.m_CommandBuffers) }
{
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
{
	m_CommandBuffers = std::move(other.m_CommandBuffers);
	return *this;
}

void CommandBuffer::Start() const
{
	const VkCommandBuffer commandBuffer{ GetBuffer() };

	// reset command buffer before filling up
	vkResetCommandBuffer(commandBuffer, 0);

	// Start command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin recording command buffer");
}

void CommandBuffer::End() const
{
	if (vkEndCommandBuffer(GetBuffer()) != VK_SUCCESS)
		throw std::runtime_error("Failed to record command buffer");
}

const VkCommandBuffer& CommandBuffer::GetBuffer() const
{
	return  m_CommandBuffers[VulkanBase::GetInstance().GetSwapChain().GetCurrentFrame()];
}
