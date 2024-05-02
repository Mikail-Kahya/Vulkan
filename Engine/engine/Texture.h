#pragma once
#include <memory>
#include <string>

#include <vulkan/vulkan_core.h>

namespace mk
{
	class Texture final
	{
	public:
		Texture(const std::string& path);
		~Texture();

		Texture(const Texture& other) = delete;
		Texture(Texture&& other) noexcept = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) noexcept = delete;

	private:
		void CreateImage(VkDevice device);
		void CreateMemory(VkDevice device, VkPhysicalDevice physicalDevice);

		uint32_t m_Width{};
		uint32_t m_Height{};

		VkImage m_TextureImage{};
		VkDeviceMemory M_TextureImageMemory{};
	};
}