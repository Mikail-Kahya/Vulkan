#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

Texture::Texture(const std::string& path)
{
    CreateImage(path);
    CreateImageView();
    CreateTextureSampler();
}

Texture::~Texture()
{
    const VkDevice device{ VulkanBase::GetInstance().GetDevice() };
    if (m_TextureSampler != VK_NULL_HANDLE)
		vkDestroySampler(device, m_TextureSampler, nullptr);

    if (m_ImageView != VK_NULL_HANDLE)
		vkDestroyImageView(device, m_ImageView, nullptr);

    if (m_Image != VK_NULL_HANDLE)
		vkDestroyImage(device, m_Image, nullptr);

    if (m_ImageMemory != VK_NULL_HANDLE)
		vkFreeMemory(device, m_ImageMemory, nullptr);
}

Texture::Texture(Texture&& other) noexcept
    : m_Width{ other.m_Width }
    , m_Height{ other.m_Height }
    , m_Image{ other.m_Image }
    , m_ImageMemory{ other.m_ImageMemory }
    , m_ImageView{ other.m_ImageView }
    , m_TextureSampler{ other.m_TextureSampler }
{
    other.m_Image = VK_NULL_HANDLE;
    other.m_ImageMemory = VK_NULL_HANDLE;
    other.m_ImageView = VK_NULL_HANDLE;
    other.m_TextureSampler = VK_NULL_HANDLE;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Image = other.m_Image;
    m_ImageMemory = other.m_ImageMemory;
    m_ImageView = other.m_ImageView;
    m_TextureSampler = other.m_TextureSampler;

    other.m_Image = VK_NULL_HANDLE;
    other.m_ImageMemory = VK_NULL_HANDLE;
    other.m_ImageView = VK_NULL_HANDLE;
    other.m_TextureSampler = VK_NULL_HANDLE;

    return *this;
}

VkDescriptorImageInfo Texture::GetImageInfo() const
{
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = m_ImageView;
    imageInfo.sampler = m_TextureSampler;
    return imageInfo;
}

void Texture::CreateImage(const std::string& path)
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
    CreateBuffer(device, physicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    stbi_image_free(pixels);

    mk::CreateImage(device, physicalDevice, m_Width, m_Height, 
					VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, 
					VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
					m_Image, m_ImageMemory);

    const CommandPool& pool{ app.GetCommandPool() };
    pool.TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    pool.CopyBufferToImage(stagingBuffer, m_Image, m_Width, m_Height);
    pool.TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


    // cleanup
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Texture::CreateImageView()
{
    m_ImageView = CreateVkImageView(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Texture::CreateTextureSampler()
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(VulkanBase::GetInstance().GetPhysicalDevice(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    // Anisotropic
    //samplerInfo.anisotropyEnable = VK_TRUE;
    //samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.f;

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    // Mipmaps
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(VulkanBase::GetInstance().GetDevice(), &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS)
        throw std::runtime_error("Failed to create texture sampler");
}