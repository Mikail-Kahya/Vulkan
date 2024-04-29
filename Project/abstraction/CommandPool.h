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
		CommandBuffer CreatePrimaryBuffer() const;
		SecondaryCommandBuffer CreateSecondaryBuffer() const;

	private:
		std::vector<VkCommandBuffer> CreateCommandBuffer(int nrBuffers = 1, VkCommandBufferLevel bufferType = VK_COMMAND_BUFFER_LEVEL_PRIMARY) const;

		VkCommandBuffer BeginSingleTimeCommands() const;
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

		VkCommandPool m_CommandPool{ VK_NULL_HANDLE };
	};
}
