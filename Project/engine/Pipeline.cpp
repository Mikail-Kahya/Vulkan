#include "Pipeline.h"

#include <stdexcept>
#include <vector>

#include "vulkanbase/VulkanBase.h"

using namespace mk;

Pipeline::~Pipeline()
{
	if (!m_Destroyed)
		Destroy();
}

Pipeline::Pipeline(Pipeline&& other) noexcept
	: m_Shader{ std::move(other.m_Shader) }
	, m_PipelineLayout{ other.m_PipelineLayout }
	, m_RenderPass{ other.m_RenderPass }
	, m_GraphicsPipeline{ other.m_GraphicsPipeline }
	, m_SwapChainFramebuffers{ std::move(other.m_SwapChainFramebuffers) }
	, m_CommandBuffers{ std::move(other.m_CommandBuffers) }
	, m_ClearColor{ other.m_ClearColor }
	, m_Destroyed{ other.m_Destroyed }
{
	other.m_PipelineLayout = nullptr;
	other.m_RenderPass = nullptr;
	other.m_GraphicsPipeline = nullptr;
	other.m_Destroyed = true;
}

Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
{
	m_Shader = std::move(other.m_Shader);
	m_PipelineLayout = other.m_PipelineLayout;
	m_RenderPass = other.m_RenderPass;
	m_GraphicsPipeline = other.m_GraphicsPipeline;
	m_SwapChainFramebuffers = std::move(other.m_SwapChainFramebuffers);
	m_CommandBuffers = std::move(other.m_CommandBuffers);
	m_ClearColor = other.m_ClearColor;
	m_Destroyed = other.m_Destroyed;

	other.m_PipelineLayout = nullptr;
	other.m_RenderPass = nullptr;
	other.m_GraphicsPipeline = nullptr;
	other.m_Destroyed = true;

	return *this;
}

void Pipeline::Initialize(const std::string& shaderName)
{
	m_Destroyed = false;

	m_Shader = std::make_unique<Shader>(shaderName, shaderName);
	m_Shader->Initialize(VulkanBase::GetInstance().GetDevice());
	CreatePipelineLayout();
	CreateRenderPass();
	CreatePipeline();
	m_CommandBuffers = VulkanBase::GetInstance().GetCommandPool().CreateCommandBuffer(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	CreateBuffers();
}

void Pipeline::Destroy()
{
	m_Destroyed = true;

	const VkDevice& device{ VulkanBase::GetInstance().GetDevice() };

	for (auto framebuffer : m_SwapChainFramebuffers)
		vkDestroyFramebuffer(device, framebuffer, nullptr);

	m_Shader->DestroyModules(device);
	vkDestroyPipeline(device, m_GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, m_PipelineLayout,nullptr);
	vkDestroyRenderPass(device, m_RenderPass, nullptr);
}

void Pipeline::Update()
{
	const VkDevice& device{ VulkanBase::GetInstance().GetDevice() };
	for (auto framebuffer : m_SwapChainFramebuffers)
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	CreateBuffers();
}

void Pipeline::Draw(uint32_t imageIdx, const std::vector<Mesh*>& meshes) const
{
	const SwapChain& swapChain{ VulkanBase::GetInstance().GetSwapChain() };
	const auto scissor{ swapChain.GetScissor() };
	const auto viewport{ swapChain.GetViewport() };
	const VkCommandBuffer commandBuffer{ m_CommandBuffers[swapChain.GetCurrentFrame()] };

	// reset command buffer before filling up
	vkResetCommandBuffer(commandBuffer, 0);

	// Start command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin recording command buffer");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_RenderPass;
	renderPassInfo.framebuffer = m_SwapChainFramebuffers[imageIdx];
	renderPassInfo.renderArea = scissor;
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &m_ClearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	for (Mesh* mesh : meshes)
		mesh->Draw(commandBuffer);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to record command buffer");

	SubmitCommandBuffer();
}

void Pipeline::CreatePipelineLayout()
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

void Pipeline::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = VulkanBase::GetInstance().GetSwapChain().GetSwapChainImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(VulkanBase::GetInstance().GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		throw std::runtime_error("Failed to create render pass");
}

void Pipeline::CreatePipeline()
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
	pipelineInfo.renderPass = m_RenderPass;
	pipelineInfo.subpass = 0;

	// for derived pipelines
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // optional
	pipelineInfo.basePipelineIndex = -1; // optional

	if (vkCreateGraphicsPipelines(VulkanBase::GetInstance().GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
		throw std::runtime_error("Failed to create graphics pipeline");
}

void Pipeline::CreateBuffers()
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	const SwapChain& swapChain{ vulkanBase.GetSwapChain() };
	m_SwapChainFramebuffers.resize(swapChain.GetNrImagesViews());

	for (int idx{}; idx < swapChain.GetNrImagesViews(); ++idx)
	{
		VkImageView attachments[]{ swapChain.GetImageView(idx) };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_RenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChain.GetWidth();
		framebufferInfo.height = swapChain.GetHeight();
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(vulkanBase.GetDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[idx]) != VK_SUCCESS)
			throw std::runtime_error("Failed to create framebuffer");
	}
}

void Pipeline::SubmitCommandBuffer() const
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	const SwapChain& swapChain{ vulkanBase.GetSwapChain() };
	const auto waitSemaphores{ swapChain.GetWaitSemaphores() };
	const auto signalSemaphores{ swapChain.GetSignalSemaphores() };
	constexpr VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	submitInfo.pSignalSemaphores = signalSemaphores.data();
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffers[swapChain.GetCurrentFrame()];

	if (vkQueueSubmit(vulkanBase.GetGraphicsQueue(), 1, &submitInfo, swapChain.GetWaitingFence()))
		throw std::runtime_error("Failed to submit draw command buffer");
}

VkPipelineDynamicStateCreateInfo Pipeline::CreateDynamicState(const std::vector<VkDynamicState>& dynamicStates)
{
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();
	return dynamicState;
}

VkPipelineVertexInputStateCreateInfo Pipeline::CreateVertexInfo()
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

VkPipelineInputAssemblyStateCreateInfo Pipeline::CreateInputAssembly()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkPipelineViewportStateCreateInfo Pipeline::CreateViewportState()
{
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;
	return viewportState;
}

VkPipelineRasterizationStateCreateInfo Pipeline::CreateRasterizer()
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

VkPipelineMultisampleStateCreateInfo Pipeline::CreateMultisampling()
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

VkPipelineColorBlendAttachmentState Pipeline::CreateColorBlendAttachment()
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	//colorBlendAttachment.blendEnable = VK_TRUE;
	//colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	//colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	//colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	//colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	//colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	//colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	return colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo Pipeline::CreateColorBlend(VkPipelineColorBlendAttachmentState* colorBlendAttachment)
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