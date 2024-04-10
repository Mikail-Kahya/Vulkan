#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#include "Mesh3D.h"
#include "Shader.h"

namespace mk
{
	class Pipeline3D final
	{
		using Vertex = Vertex3D;
		using Mesh = Mesh3D;
	public:
		Pipeline3D() = default;
		~Pipeline3D();

		Pipeline3D(const Pipeline3D& other)					= delete;
		Pipeline3D(Pipeline3D&& other) noexcept;
		Pipeline3D& operator=(const Pipeline3D& other)		= delete;
		Pipeline3D& operator=(Pipeline3D&& other) noexcept;

		void Initialize(const std::string& shaderName);
		void Destroy();
		void Update();

		void Draw(uint32_t imageIdx, const std::vector<Mesh*>& meshes) const;

	private:
		void CreatePipelineLayout();
		void CreateRenderPass();
		void CreatePipeline();
		void CreateBuffers();

		void SubmitCommandBuffer() const;

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
		VkPipeline m_GraphicsPipeline{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer> m_SwapChainFramebuffers{};

		std::vector<VkCommandBuffer> m_CommandBuffers{};
		VkClearValue m_ClearColor{ {{0.0f, 0.0f, 0.0f, 1.0f}} };
	};
}