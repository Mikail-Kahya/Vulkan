#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class CommandBuffer final
	{
	public:
		CommandBuffer(std::vector<VkCommandBuffer>&& commandBuffers);
		~CommandBuffer() = default;

		CommandBuffer(const CommandBuffer& other)					= delete;
		CommandBuffer(CommandBuffer&& other) noexcept;
		CommandBuffer& operator=(const CommandBuffer& other)		= delete;
		CommandBuffer& operator=(CommandBuffer&& other) noexcept;

		void Start() const;
		void End() const;

		const VkCommandBuffer& GetBuffer() const;

	private:
		std::vector<VkCommandBuffer> m_CommandBuffers{};
	};
}
