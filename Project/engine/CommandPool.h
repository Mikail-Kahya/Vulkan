#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class CommandBuffer;

	class CommandPool final
	{
	public:
		CommandPool() = default;
		~CommandPool();

		CommandPool(const CommandPool& other)					= delete;
		CommandPool(CommandPool&& other) noexcept				= delete;
		CommandPool& operator=(const CommandPool& other)		= delete;
		CommandPool& operator=(CommandPool&& other) noexcept	= delete;

		void Initialize();
		void Destroy();

		std::vector<VkCommandBuffer> CreateCommandBuffer(int nrBuffers = 1) const;

	private:
		void CreateCommandPool();

		VkCommandPool m_CommandPool{ VK_NULL_HANDLE };
		bool m_Destroy{ true };
	};
}
