#include "Pipeline2D.h"

#include <stdexcept>
#include <vector>

#include "SecondaryCommandBuffer.h"
#include "vulkanbase/VulkanBase.h"

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

void Pipeline2D::Initialize(const std::string& shaderName)
{
	m_Shader = std::make_unique<Shader>(shaderName, shaderName);
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

void Pipeline2D::Draw(const std::vector<Mesh*>& meshes) const
{
	const VulkanBase& app{ VulkanBase::GetInstance() };
	const SwapChain& swapChain{ app.GetSwapChain() };
	const RenderPass& renderPass{ app.GetRenderPass() };
	const auto scissor{ swapChain.GetScissor() };
	const auto viewport{ swapChain.GetViewport() };

	m_CommandBuffer->Start(renderPass);
	const VkCommandBuffer commandBuffer{ m_CommandBuffer->GetBuffer() };
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	for (Mesh* mesh : meshes)
		mesh->Draw(m_CommandBuffer->GetBuffer());

	m_CommandBuffer->End(renderPass);
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
	VkPipelineVertexInputStateCreateInfo vertexInfo{ CreateVertexInfo() };
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{ CreateInputAssembly() };
	VkPipelineRasterizationStateCreateInfo rasterizer{ CreateRasterizer() };
	VkPipelineMultisampleStateCreateInfo multisampling{ CreateMultisampling() };
	VkPipelineColorBlendAttachmentState colorBlendAttachment{ CreateColorBlendAttachment() };
	VkPipelineColorBlendStateCreateInfo colorBlending{ CreateColorBlend(&colorBlendAttachment) };

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<uint32_t>(m_Shader->GetShaderStages().size());
	pipelineInfo.pStages = m_Shader->GetShaderStages().data();

	pipelineInfo.pVertexInputState = &vertexInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
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

VkPipelineDynamicStateCreateInfo Pipeline2D::CreateDynamicState(const std::vector<VkDynamicState>& dynamicStates)
{
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();
	return dynamicState;
}

VkPipelineVertexInputStateCreateInfo Pipeline2D::CreateVertexInfo()
{
	const auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = Vertex::GetBindingDescription();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions->size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions->data();
	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo Pipeline2D::CreateInputAssembly()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkPipelineViewportStateCreateInfo Pipeline2D::CreateViewportState()
{
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;
	return viewportState;
}

VkPipelineRasterizationStateCreateInfo Pipeline2D::CreateRasterizer()
{
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0; // Optional
	rasterizer.depthBiasClamp = 0; // Optional
	rasterizer.depthBiasSlopeFactor = 0; // Optional
	return rasterizer;
}

VkPipelineMultisampleStateCreateInfo Pipeline2D::CreateMultisampling()
{
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional
	return multisampling;
}

VkPipelineColorBlendAttachmentState Pipeline2D::CreateColorBlendAttachment()
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	return colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo Pipeline2D::CreateColorBlend(VkPipelineColorBlendAttachmentState* colorBlendAttachment)
{
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	return colorBlending;
}