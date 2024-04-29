#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#include "engine/Mesh3D.h"
#include "Shader.h"

namespace mk
{
	class SecondaryCommandBuffer;

	class Pipeline3D final
	{
		using Vertex = Vertex3D;
		using Mesh = Mesh3D;
	public:
		Pipeline3D();
		~Pipeline3D();

		Pipeline3D(const Pipeline3D& other) = delete;
		Pipeline3D(Pipeline3D&& other) noexcept;
		Pipeline3D& operator=(const Pipeline3D& other) = delete;
		Pipeline3D& operator=(Pipeline3D&& other) noexcept;

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

		inline static const VkClearValue CLEAR_COLOR{ {{0.0f, 0.0f, 0.5f, 1.0f} } };

		std::unique_ptr<Shader> m_Shader;
		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_GraphicsPipeline{ VK_NULL_HANDLE };
		std::unique_ptr<SecondaryCommandBuffer> m_CommandBuffer;
	};
}
