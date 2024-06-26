#include "Pipeline2D.h"

#include <stdexcept>
#include <vector>

#include "SecondaryCommandBuffer.h"
#include "vulkan/VulkanBase.h"

using namespace mk;

Pipeline2D::Pipeline2D()
	: m_CommandBuffer{}
{
}

Pipeline2D::~Pipeline2D()
{
	Destroy();
}

Pipeline2D::Pipeline2D(Pipeline2D&& other) noexcept
	: m_Shader{ std::move(other.m_Shader) }
	, m_PipelineLayout{ other.m_PipelineLayout }
	, m_GraphicsPipeline{ other.m_GraphicsPipeline }
	, m_CommandBuffer{ std::move(other.m_CommandBuffer) }
{
	other.m_PipelineLayout = nullptr;
	other.m_GraphicsPipeline = nullptr;
}

Pipeline2D& Pipeline2D::operator=(Pipeline2D&& other) noexcept
{
	m_Shader = std::move(other.m_Shader);
	m_PipelineLayout = other.m_PipelineLayout;
	m_GraphicsPipeline = other.m_GraphicsPipeline;
	m_CommandBuffer = std::move(other.m_CommandBuffer);

	other.m_PipelineLayout = nullptr;
	other.m_GraphicsPipeline = nullptr;

	return *this;
}

void Pipeline2D::Initialize(const std::string& vertex, const std::string& fragment)
{
	m_Shader = std::make_unique<Shader>(vertex, fragment);
	m_Shader->Initialize(VulkanBase::GetInstance().GetDevice());
	CreatePipelineLayout();
	CreatePipeline();
	m_CommandBuffer = std::make_unique<SecondaryCommandBuffer>(VulkanBase::GetInstance().GetCommandPool().CreateSecondaryBuffer());
}

void Pipeline2D::Destroy()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	if (m_Shader)
		m_Shader->DestroyModules(device);

	if (m_GraphicsPipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(device, m_GraphicsPipeline, nullptr);
		m_GraphicsPipeline = VK_NULL_HANDLE;
	}

	if (m_PipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
		m_PipelineLayout = VK_NULL_HANDLE;
	}
}

void Pipeline2D::StartDrawing()
{
	const VulkanBase& app{ VulkanBase::GetInstance() };
	const SwapChain& swapChain{ app.GetSwapChain() };
	const RenderPass& renderPass{ app.GetRenderPass() };
	const auto scissor{ swapChain.GetScissor() };
	const auto viewport{ swapChain.GetViewport() };

	//m_CommandBuffer->Start(renderPass);
	//const VkCommandBuffer commandBuffer{ m_CommandBuffer->GetBuffer() };
	const VkCommandBuffer commandBuffer{ renderPass.GetPrimaryBuffer() };
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	m_DrawCommandBuffer = commandBuffer;
}

void Pipeline2D::Draw(Mesh* meshPtr) const
{
	if (m_DrawCommandBuffer == VK_NULL_HANDLE)
		return;

	meshPtr->Draw(m_DrawCommandBuffer);
}

void Pipeline2D::EndDrawing()
{
	//m_CommandBuffer->End(renderPass);
	m_DrawCommandBuffer = VK_NULL_HANDLE;
}

void Pipeline2D::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(VulkanBase::GetInstance().GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout!");
}

void Pipeline2D::CreatePipeline()
{
	const std::vector<VkDynamicState> dynamicStates{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{ CreateDynamicState(dynamicStates) };
	VkPipelineViewportStateCreateInfo viewportState{ CreateViewportState() };
	VkPipelineVertexInputStateCreateInfo vertexInfo{ CreateVertexInfo<Vertex2D>() };
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{ CreateInputAssembly() };
	VkPipelineRasterizationStateCreateInfo rasterizer{ CreateRasterizer() };
	VkPipelineMultisampleStateCreateInfo multisampling{ CreateMultisampling() };
	VkPipelineColorBlendAttachmentState colorBlendAttachment{ CreateColorBlendAttachment() };
	VkPipelineColorBlendStateCreateInfo colorBlending{ CreateColorBlend(&colorBlendAttachment) };
	VkPipelineDepthStencilStateCreateInfo depthStencil{ CreateDepthStencilInfo() };

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<uint32_t>(m_Shader->GetShaderStages().size());
	pipelineInfo.pStages = m_Shader->GetShaderStages().data();

	pipelineInfo.pVertexInputState = &vertexInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;

	pipelineInfo.layout = m_PipelineLayout;
	pipelineInfo.renderPass = VulkanBase::GetInstance().GetRenderPass().GetVkRenderPass();
	pipelineInfo.subpass = 0;

	// for derived pipelines
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // optional
	pipelineInfo.basePipelineIndex = -1; // optional

	if (vkCreateGraphicsPipelines(VulkanBase::GetInstance().GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
		throw std::runtime_error("Failed to create graphics pipeline");
}