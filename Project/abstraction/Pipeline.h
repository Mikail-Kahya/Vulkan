#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#include "Shader.h"

namespace mk
{
	class Pipeline final
	{
	public:
		Pipeline() = default;
		~Pipeline();

		Pipeline(const Pipeline& other)					= delete;
		Pipeline(Pipeline&& other) noexcept				= delete;
		Pipeline& operator=(const Pipeline& other)		= delete;
		Pipeline& operator=(Pipeline&& other) noexcept	= delete;

		void Initialize(const std::string& shaderName);
		void Destroy();

	private:
		void CreatePipelineLayout();
		void CreateRenderPass();
		void CreatePipeline();
		void CreateBuffers();

		// Fixed functions
		static VkPipelineDynamicStateCreateInfo CreateDynamicState(const std::vector<VkDynamicState>& dynamicStates);
		static VkPipelineVertexInputStateCreateInfo CreateVertexInfo();
		static VkPipelineInputAssemblyStateCreateInfo CreateInputAssembly();
		static VkPipelineViewportStateCreateInfo CreateViewportState();
		static VkPipelineRasterizationStateCreateInfo CreateRasterizer();
		static VkPipelineMultisampleStateCreateInfo CreateMultisampling();
		static VkPipelineColorBlendAttachmentState CreateColorBlendAttachment();
		static VkPipelineColorBlendStateCreateInfo CreateColorBlend(VkPipelineColorBlendAttachmentState* colorBlendAttachment);

		std::unique_ptr<Shader> m_Shader;
		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkRenderPass m_RenderPass{ VK_NULL_HANDLE };
		VkPipeline m_Pipeline{};
		std::vector<VkFramebuffer> m_SwapChainFramebuffers{};
		bool m_Destroyed{ true };
	};
}