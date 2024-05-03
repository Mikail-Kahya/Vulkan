#pragma once
#include <memory>
#include <string>

#include <vulkan/vulkan_core.h>

namespace mk
{
	class Texture final
	{
	public:
		Texture() = default;
		Texture(const std::string& path);
		~Texture();

		Texture(const Texture& other)					= delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(const Texture& other)		= delete;
		Texture& operator=(Texture&& other) noexcept;

		VkDescriptorImageInfo GetImageInfo() const;

	private:
		void CreateImage(const std::string& path);
		void CreateImageView();
		void CreateTextureSampler();
		void CreateVkImage(VkDevice device);
		void CreateVkMemory(VkDevice device, VkPhysicalDevice physicalDevice);

		uint32_t m_Width{};
		uint32_t m_Height{};

		VkImage m_Image{};
		VkDeviceMemory m_ImageMemory{};
		VkImageView m_ImageView{};
		VkSampler m_TextureSampler{};
	};
}
