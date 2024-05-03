#include "DepthBuffer.h"

#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

DepthBuffer::DepthBuffer()
{
    const auto& app{ VulkanBase::GetInstance() };
    const SwapChain& swapChain{ app.GetSwapChain() };

    const VkFormat depthFormat{ FindDepthFormat(app.GetPhysicalDevice()) };
    CreateImage(app.GetDevice(), app.GetPhysicalDevice(), swapChain.GetWidth(), swapChain.GetHeight(), depthFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        m_Image, m_ImageMemory);
    m_ImageView = CreateVkImageView(m_Image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    app.GetCommandPool().TransitionImageLayout(m_Image, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

DepthBuffer::~DepthBuffer()
{
    const VkDevice device{ VulkanBase::GetInstance().GetDevice() };

    if (m_ImageView != VK_NULL_HANDLE)
        vkDestroyImageView(device, m_ImageView, nullptr);

    if (m_Image != VK_NULL_HANDLE)
        vkDestroyImage(device, m_Image, nullptr);

    if (m_ImageMemory != VK_NULL_HANDLE)
        vkFreeMemory(device, m_ImageMemory, nullptr);
}

DepthBuffer::DepthBuffer(DepthBuffer&& other) noexcept
    : m_Image{ other.m_Image }
    , m_ImageMemory{ other.m_ImageMemory }
    , m_ImageView{ other.m_ImageView }
{
    other.m_Image = VK_NULL_HANDLE;
    other.m_ImageMemory = VK_NULL_HANDLE;
    other.m_ImageView = VK_NULL_HANDLE;
}

DepthBuffer& DepthBuffer::operator=(DepthBuffer&& other) noexcept
{
    m_Image = other.m_Image;
    m_ImageMemory = other.m_ImageMemory;
    m_ImageView = other.m_ImageView;

    other.m_Image = VK_NULL_HANDLE;
    other.m_ImageMemory = VK_NULL_HANDLE;
    other.m_ImageView = VK_NULL_HANDLE;

    return *this;
}

VkImageView DepthBuffer::GetView() const
{
    return m_ImageView;
}
