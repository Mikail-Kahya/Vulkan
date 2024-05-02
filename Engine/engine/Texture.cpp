#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

Texture::Texture(const std::string& path)
{
    int32_t channels{};
    int32_t width{};
    int32_t height{};
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    m_Width = static_cast<uint32_t>(width);
    m_Height = static_cast<uint32_t>(height);

    const VkDeviceSize imageSize{ m_Width * m_Height * 4 };

    if (pixels == nullptr)
        throw std::runtime_error("Failed to load texture image: " + path);

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory{};

	const auto& app = VulkanBase::GetInstance();
    const VkDevice device{ app.GetDevice() };
    const VkPhysicalDevice physicalDevice{ app.GetPhysicalDevice() };
    CreateBuffer(   device, physicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    stbi_image_free(pixels);

    CreateImage(device);
    CreateMemory(device, physicalDevice);
    vkBindImageMemory(device, m_TextureImage, M_TextureImageMemory, 0);

    const CommandPool& pool{ app.GetCommandPool() };
    pool.TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    pool.CopyBufferToImage(stagingBuffer, m_TextureImage, m_Width, m_Height);
    pool.TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


    // cleanup
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

Texture::~Texture()
{
    const VkDevice device{ VulkanBase::GetInstance().GetDevice() };
    vkDestroyImage(device, m_TextureImage, nullptr);
    vkFreeMemory(device, M_TextureImageMemory, nullptr);
}

void Texture::CreateImage(VkDevice device)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(m_Width);
    imageInfo.extent.height = static_cast<uint32_t>(m_Height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional

    if (vkCreateImage(device, &imageInfo, nullptr, &m_TextureImage) != VK_SUCCESS)
        throw std::runtime_error("Failed to create image");
}

void Texture::CreateMemory(VkDevice device, VkPhysicalDevice physicalDevice)
{
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, m_TextureImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &M_TextureImageMemory) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate image memory!");
}
