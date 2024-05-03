#include "SwapChain.h"

#include <cassert>
#include <iostream>
#include <vulkan/vulkan_core.h>

#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

SwapChain::SwapChain()
{
	Initialize();
}

SwapChain::~SwapChain()
{
	Destroy();
}

void SwapChain::Initialize()
{
	CreateSwapChain();
	CreateImageViews();
	CreateSyncObjects();
}

void SwapChain::Destroy()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	for (int idx{}; idx < VulkanBase::MAX_FRAMES_IN_FLIGHT; ++idx)
	{
		if (!m_ImageAvailableSemaphores.empty())
			vkDestroySemaphore(device, m_ImageAvailableSemaphores[idx], nullptr);
			
		if (!m_RenderFinishedSemaphores.empty())
			vkDestroySemaphore(device, m_RenderFinishedSemaphores[idx], nullptr);
			
		if (!m_InFlightFences.empty())
			vkDestroyFence(device, m_InFlightFences[idx], nullptr);
	}

	m_ImageAvailableSemaphores.clear();
	m_RenderFinishedSemaphores.clear();
	m_InFlightFences.clear();

	for (auto imageView : m_SwapChainImageViews)
		vkDestroyImageView(device, imageView, nullptr);

	m_SwapChainImageViews.clear();

	if (m_SwapChain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
		m_SwapChain = VK_NULL_HANDLE;
	}
		
}

void SwapChain::Wait()
{
	const VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	const VkResult result = vkWaitForFences(device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
		UpdateInternal();
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("Failed to acquire swap chain image");

	vkResetFences(device, 1, &m_InFlightFences[m_CurrentFrame]);
}

void SwapChain::Present(uint32_t imageIdx)
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	const auto signalSemaphores{ GetSignalSemaphores() };
	presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	presentInfo.pWaitSemaphores = signalSemaphores.data();

	const VkSwapchainKHR swapChains[] = { m_SwapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIdx;
	presentInfo.pResults = nullptr; // Optional

	const VkResult result = vkQueuePresentKHR(VulkanBase::GetInstance().GetPresentQueue(), &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		UpdateInternal();
	else if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to present swap chain image");
}

void SwapChain::NextFrame()
{
	m_CurrentFrame = (m_CurrentFrame + 1) % VulkanBase::MAX_FRAMES_IN_FLIGHT;
}

void SwapChain::UpdateInternal()
{
	VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	int width, height{};
	glfwGetFramebufferSize(vulkanBase.GetWindow(), &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(vulkanBase.GetWindow(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(VulkanBase::GetInstance().GetDevice());
	vulkanBase.WindowChanged();
	Destroy();
	Initialize();
}

const VkFormat& SwapChain::GetSwapChainImageFormat() const
{
	return m_SwapChainImageFormat;
}

uint32_t SwapChain::GetWidth() const
{
	return m_SwapChainExtent.width;
}

uint32_t SwapChain::GetHeight() const
{
	return m_SwapChainExtent.height;
}

VkViewport SwapChain::GetViewport() const
{
	VkViewport viewport{};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = static_cast<float>(GetWidth());
	viewport.height = static_cast<float>(GetHeight());
	viewport.minDepth = 0;
	viewport.maxDepth = 1;

	return viewport;
}

VkRect2D SwapChain::GetScissor() const
{
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = m_SwapChainExtent;
	return scissor;
}

int SwapChain::GetNrImagesViews() const
{
	return static_cast<int>(m_SwapChainImageViews.size());
}

VkImageView SwapChain::GetImageView(uint32_t idx) const
{
	if (idx >= static_cast<uint32_t>(GetNrImagesViews()))
	{
		assert(false && "Out of range");
		return {};
	}

	return m_SwapChainImageViews[idx];
}

uint32_t SwapChain::GetImageIdx()
{
	uint32_t imageIdx{};
	const VkResult result = vkAcquireNextImageKHR(VulkanBase::GetInstance().GetDevice(), m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIdx);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
		UpdateInternal();
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("Failed to acquire swap chain image");

	return imageIdx;
}

uint32_t SwapChain::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

std::vector<VkSemaphore> SwapChain::GetWaitSemaphores() const
{
	return { m_ImageAvailableSemaphores[m_CurrentFrame] };
}

std::vector<VkSemaphore> SwapChain::GetSignalSemaphores() const
{
	return { m_RenderFinishedSemaphores[m_CurrentFrame] };
}

VkFence SwapChain::GetWaitingFence() const
{
	return m_InFlightFences[m_CurrentFrame];
}

void SwapChain::CreateSwapChain()
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkPhysicalDevice physicalDevice{ vulkanBase.GetPhysicalDevice() };
	VkDevice device{ vulkanBase.GetDevice() };
	const VkSurfaceKHR surface{ vulkanBase.GetSurface() };

	SwapChainSupportDetails swapChainSupport{ QuerySwapChainSupport(physicalDevice, surface) };

	VkSurfaceFormatKHR surfaceFormat{ ChooseSwapSurfaceFormat(swapChainSupport.formats) };
	VkPresentModeKHR presentMode{ ChooseSwapPresentMode(swapChainSupport.presentModes) };
	VkExtent2D extent{ ChooseSwapExtent2D(swapChainSupport.capabilities, vulkanBase.GetWindow()) };

	uint32_t imageCount{ swapChainSupport.capabilities.minImageCount + 1 };
	if (swapChainSupport.capabilities.maxImageCount > 9 &&
		imageCount > swapChainSupport.capabilities.maxImageCount)
		imageCount = swapChainSupport.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices{ FindQueueFamilies(physicalDevice, surface) };
	uint32_t queueFamiliyIndices[]{ indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamiliyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
		throw std::runtime_error("Failed to create swap chain");

	// Get images
	vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, nullptr);
	m_SwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, m_SwapChainImages.data());
	m_SwapChainImageFormat = surfaceFormat.format;
	m_SwapChainExtent = extent;
}

void SwapChain::CreateImageViews()
{
	m_SwapChainImageViews.resize(m_SwapChainImages.size());

	for (size_t idx{}; idx < m_SwapChainImages.size(); ++idx)
		m_SwapChainImageViews[idx] = CreateVkImageView(m_SwapChainImages[idx], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

void SwapChain::CreateSyncObjects()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };
	m_ImageAvailableSemaphores.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	m_RenderFinishedSemaphores.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	m_InFlightFences.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int idx{}; idx < VulkanBase::MAX_FRAMES_IN_FLIGHT; ++idx)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[idx]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[idx]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[idx]) != VK_SUCCESS)
			throw std::runtime_error("Failed to create synchronization objects for frame " + std::to_string(idx));
	}
}
