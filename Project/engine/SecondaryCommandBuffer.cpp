#include "SecondaryCommandBuffer.h"
#include <stdexcept>

#include "vulkanbase/VulkanBase.h"

using namespace mk;

SecondaryCommandBuffer::SecondaryCommandBuffer(std::vector<VkCommandBuffer>&& commandBuffers)
	: m_CommandBuffers{ std::move(commandBuffers) }
{
}

SecondaryCommandBuffer::SecondaryCommandBuffer(SecondaryCommandBuffer&& other) noexcept
	: m_CommandBuffers{ std::move(other.m_CommandBuffers) }
{
}

SecondaryCommandBuffer& SecondaryCommandBuffer::operator=(SecondaryCommandBuffer&& other) noexcept
{
	m_CommandBuffers = std::move(other.m_CommandBuffers);
	return *this;
}

void SecondaryCommandBuffer::Start(const RenderPass& renderPass) const
{
	const VkCommandBuffer commandBuffer{ GetBuffer() };

	// reset command buffer before filling up
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferInheritanceInfo inheritanceInfo{};
	inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	inheritanceInfo.renderPass = renderPass.GetVkRenderPass();
	inheritanceInfo.framebuffer = renderPass.GetFrameBuffer();

	// Start command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT; // Optional
	beginInfo.pInheritanceInfo = &inheritanceInfo; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin recording command buffer");
}

void SecondaryCommandBuffer::End() const
{
	if (vkEndCommandBuffer(GetBuffer()) != VK_SUCCESS)
		throw std::runtime_error("Failed to record command buffer");
}

const VkCommandBuffer& SecondaryCommandBuffer::GetBuffer() const
{
	return m_CommandBuffers[VulkanBase::GetInstance().GetSwapChain().GetCurrentFrame()];
}