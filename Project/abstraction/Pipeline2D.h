#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#include "engine/Mesh2D.h"
#include "Shader.h"


namespace mk
{
	class SecondaryCommandBuffer;

	class Pipeline2D final
	{
		using Vertex = Vertex2D;
		using Mesh = Mesh2D;
	public:
		Pipeline2D();
		~Pipeline2D();

		Pipeline2D(const Pipeline2D& other) = delete;
		Pipeline2D(Pipeline2D&& other) noexcept;
		Pipeline2D& operator=(const Pipeline2D& other) = delete;
		Pipeline2D& operator=(Pipeline2D&& other) noexcept;

		void Initialize(const std::string& shaderName);
		void Destroy();

		void Draw(const std::vector<Mesh*>& meshes) const;

	private:
		void CreatePipelineLayout();
		void CreatePipeline();

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
		VkPipeline m_GraphicsPipeline{ VK_NULL_HANDLE };
		std::unique_ptr<SecondaryCommandBuffer> m_CommandBuffer;
	};
}
