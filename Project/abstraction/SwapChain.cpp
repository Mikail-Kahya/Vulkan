#include "SwapChain.h"

#include <cassert>
#include <iostream>
#include <vulkan/vulkan_core.h>

#include "vulkanbase/VulkanBase.h"
#include "vulkanbase/VulkanUtils.h"

using namespace mk;

SwapChain::~SwapChain()
{
	if (!m_Destroyed)
		Destroy();
}

void SwapChain::Initialize()
{
	m_Destroyed = false;
	CreateSwapChain();
	CreateImageViews();
}

void SwapChain::Destroy()
{
	m_Destroyed = true;
	const VkDevice& device{ VulkanBase::GetInstance().GetDevice() };

	for (auto imageView : m_SwapChainImageViews)
		vkDestroyImageView(device, imageView, nullptr);

	vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
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

int SwapChain::GetNrImages() const
{
	return static_cast<int>(m_SwapChainImageViews.size());
}

VkImageView SwapChain::GetImage(uint32_t idx) const
{
	if (idx >= static_cast<uint32_t>(GetNrImages()))
	{
		assert(false && "Out of range");
		return {};
	}

	return m_SwapChainImageViews[idx];
}

void SwapChain::CreateSwapChain()
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	const VkPhysicalDevice& physicalDevice{ vulkanBase.GetPhysicalDevice() };
	const VkDevice& device{ vulkanBase.GetDevice() };
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
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = m_SwapChainImages[idx];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = m_SwapChainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(VulkanBase::GetInstance().GetDevice(), &createInfo, nullptr, &m_SwapChainImageViews[idx]) != VK_SUCCESS)
			throw std::runtime_error("Failed to create image views");
	}
}