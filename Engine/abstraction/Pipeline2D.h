#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#include "render/Mesh2D.h"
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

		Pipeline2D(const Pipeline2D& other)					= delete;
		Pipeline2D(Pipeline2D&& other) noexcept;
		Pipeline2D& operator=(const Pipeline2D& other)		= delete;
		Pipeline2D& operator=(Pipeline2D&& other) noexcept;

		void Initialize(const std::string& shaderName);
		void Destroy();

		void StartDrawing();
		void Draw(Mesh* meshPtr) const;
		void EndDrawing();

	private:
		void CreatePipelineLayout();
		void CreatePipeline();

		std::unique_ptr<Shader> m_Shader;
		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_GraphicsPipeline{ VK_NULL_HANDLE };
		std::unique_ptr<SecondaryCommandBuffer> m_CommandBuffer;
		VkCommandBuffer m_DrawCommandBuffer{ VK_NULL_HANDLE };
	};
}
