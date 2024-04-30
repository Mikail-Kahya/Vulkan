#include "RenderPass.h"

#include <stdexcept>

#include "CommandBuffer.h"
#include "vulkan/VulkanBase.h"

using namespace mk;

RenderPass::RenderPass()
	: m_CommandBuffer{}
{
	CreateRenderPass();
	CreateBuffers();
	m_CommandBuffer = std::make_unique<CommandBuffer>(VulkanBase::GetInstance().GetCommandPool().CreatePrimaryBuffer());
}

RenderPass::~RenderPass()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	DestroyBuffers();
	if (m_RenderPass != VK_NULL_HANDLE)
		vkDestroyRenderPass(device, m_RenderPass, nullptr);
}

RenderPass::RenderPass(RenderPass&& other) noexcept
	: m_RenderPass{ other.m_RenderPass }
	, m_SwapChainFramebuffers{ std::move(other.m_SwapChainFramebuffers) }
	, m_CommandBuffer{ std::move(other.m_CommandBuffer) }
{
	other.m_RenderPass = VK_NULL_HANDLE;
}

RenderPass& RenderPass::operator=(RenderPass&& other) noexcept
{
	m_RenderPass = other.m_RenderPass;
	m_SwapChainFramebuffers = std::move(other.m_SwapChainFramebuffers);
	m_CommandBuffer = std::move(other.m_CommandBuffer);

	other.m_RenderPass = VK_NULL_HANDLE;

	return *this;
}

void RenderPass::Update()
{
	DestroyBuffers();
	CreateBuffers();
}

void RenderPass::StartRecording(uint32_t imageIdx) const
{
	const VulkanBase& app{ VulkanBase::GetInstance() };
	m_CommandBuffer->Start();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_RenderPass;
	renderPassInfo.framebuffer = m_SwapChainFramebuffers[imageIdx];
	renderPassInfo.renderArea = app.GetSwapChain().GetScissor();
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &CLEAR_COLOR;

	//vkCmdBeginRenderPass(m_CommandBuffer->GetBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE_AND_SECONDARY_COMMAND_BUFFERS_EXT);
	vkCmdBeginRenderPass(m_CommandBuffer->GetBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::StopRecording() const
{
	vkCmdEndRenderPass(m_CommandBuffer->GetBuffer());
	m_CommandBuffer->End();
	Submit();
}

VkRenderPass RenderPass::GetVkRenderPass() const
{
	return m_RenderPass;
}

VkCommandBuffer RenderPass::GetPrimaryBuffer() const
{
	return m_CommandBuffer->GetBuffer();
}

VkFramebuffer RenderPass::GetFrameBuffer() const
{
	return m_SwapChainFramebuffers[VulkanBase::GetInstance().GetImageIdx()];
}

void RenderPass::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = VulkanBase::GetInstance().GetSwapChain().GetSwapChainImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(VulkanBase::GetInstance().GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		throw std::runtime_error("Failed to create render pass");
}

void RenderPass::CreateBuffers()
{
	const auto& app{ VulkanBase::GetInstance() };
	const SwapChain& swapChain{ app.GetSwapChain() };
	m_SwapChainFramebuffers.resize(swapChain.GetNrImagesViews());

	for (int idx{}; idx < swapChain.GetNrImagesViews(); ++idx)
	{
		VkImageView attachments[]{ swapChain.GetImageView(idx) };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_RenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChain.GetWidth();
		framebufferInfo.height = swapChain.GetHeight();
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(app.GetDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[idx]) != VK_SUCCESS)
			throw std::runtime_error("Failed to create framebuffer");
	}
}

void RenderPass::DestroyBuffers() const
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };
	for (auto framebuffer : m_SwapChainFramebuffers)
	{
		if (framebuffer != VK_NULL_HANDLE)
			vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
}

void RenderPass::Submit() const
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	const SwapChain& swapChain{ vulkanBase.GetSwapChain() };
	const auto waitSemaphores{ swapChain.GetWaitSemaphores() };
	const auto signalSemaphores{ swapChain.GetSignalSemaphores() };
	constexpr VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	submitInfo.pSignalSemaphores = signalSemaphores.data();
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffer->GetBuffer();

	if (vkQueueSubmit(vulkanBase.GetGraphicsQueue(), 1, &submitInfo, swapChain.GetWaitingFence()))
		throw std::runtime_error("Failed to submit draw command buffer");
}
