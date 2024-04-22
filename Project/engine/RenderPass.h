#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class RenderPass final
	{
	public:
		RenderPass();
		~RenderPass();

		RenderPass(const RenderPass& other)					= delete;
		RenderPass(RenderPass&& other) noexcept;
		RenderPass& operator=(const RenderPass& other)		= delete;
		RenderPass& operator=(RenderPass&& other) noexcept;

		void Update();
		void StartRecording(VkCommandBuffer commandBuffer) const;
		void StopRecording();

	private:
		void CreateRenderPass();
		void CreateBuffers();
		void DestroyBuffers() const;

		inline static const VkClearValue CLEAR_COLOR{ {{0.0f, 0.0f, 0.5f, 1.0f} } };
		VkRenderPass m_RenderPass{ VK_NULL_HANDLE };
		VkCommandBuffer m_CommandBuffer{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer> m_SwapChainFramebuffers{};
	};
}
