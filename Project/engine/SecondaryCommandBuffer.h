#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class RenderPass;

	class SecondaryCommandBuffer final
	{
	public:
		SecondaryCommandBuffer(std::vector<VkCommandBuffer>&& commandBuffers);
		~SecondaryCommandBuffer() = default;

		SecondaryCommandBuffer(const SecondaryCommandBuffer& other) = delete;
		SecondaryCommandBuffer(SecondaryCommandBuffer&& other) noexcept;
		SecondaryCommandBuffer& operator=(const SecondaryCommandBuffer& other) = delete;
		SecondaryCommandBuffer& operator=(SecondaryCommandBuffer&& other) noexcept;

		void Start(const RenderPass& renderPass) const;
		void End() const;

		const VkCommandBuffer& GetBuffer() const;

	private:
		std::vector<VkCommandBuffer> m_CommandBuffers{};
	};
}
