#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class CommandBuffer;

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
		void StartRecording(uint32_t imageIdx) const;
		void StopRecording() const;

		VkRenderPass GetVkRenderPass() const;
		VkCommandBuffer GetPrimaryBuffer() const;
		VkFramebuffer GetFrameBuffer() const;

	private:
		void CreateRenderPass();
		void CreateBuffers();
		void DestroyBuffers() const;

		void Submit() const;

		VkRenderPass m_RenderPass{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer> m_SwapChainFramebuffers{};
		std::unique_ptr<CommandBuffer> m_CommandBuffer;
	};
}
