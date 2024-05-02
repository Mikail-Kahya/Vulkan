#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class CommandBuffer;
	class SecondaryCommandBuffer;

	class CommandPool final
	{
	public:
		CommandPool();
		~CommandPool();

		CommandPool(const CommandPool& other)					= delete;
		CommandPool(CommandPool&& other) noexcept				= delete;
		CommandPool& operator=(const CommandPool& other)		= delete;
		CommandPool& operator=(CommandPool&& other) noexcept	= delete;

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const;
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const;
		CommandBuffer CreatePrimaryBuffer() const;
		SecondaryCommandBuffer CreateSecondaryBuffer() const;

	private:
		std::vector<VkCommandBuffer> CreateCommandBuffer(int nrBuffers = 1, VkCommandBufferLevel bufferType = VK_COMMAND_BUFFER_LEVEL_PRIMARY) const;

		VkCommandBuffer BeginSingleTimeCommands() const;
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

		VkCommandPool m_CommandPool{ VK_NULL_HANDLE };
	};
}
