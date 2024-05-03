#pragma once
#include <vulkan/vulkan_core.h>

namespace mk
{
	class DepthBuffer final
	{
	public:
		DepthBuffer();
		~DepthBuffer();

		DepthBuffer(const DepthBuffer& other)					= delete;
		DepthBuffer(DepthBuffer&& other) noexcept;
		DepthBuffer& operator=(const DepthBuffer& other)		= delete;
		DepthBuffer& operator=(DepthBuffer&& other) noexcept;

		VkImageView GetView() const;

	private:
		VkImage m_Image{};
		VkDeviceMemory m_ImageMemory{};
		VkImageView m_ImageView{};
	};
}