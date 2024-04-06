#include "Shader.h"

#include <numeric>
#include <stdexcept>

#include "vulkanbase/VulkanUtils.h"

using namespace mk;

Shader::Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	: m_VertexShaderFile{ vertexShaderFile }
	, m_FragmentShaderFile{ fragmentShaderFile }
{
}

void Shader::Initialize(const VkDevice& vkDevice)
{
	m_ShaderStages.emplace_back(CreateVertexShaderInfo(vkDevice, m_VertexShaderFile));
	m_ShaderStages.emplace_back(CreateFragmentShaderInfo(vkDevice, m_FragmentShaderFile));
}

const std::vector<VkPipelineShaderStageCreateInfo>& Shader::GetShaderStages() const
{
	return m_ShaderStages;
}

void Shader::DestroyModules(const VkDevice& vkDevice)
{
	for (VkPipelineShaderStageCreateInfo& stageInfo : m_ShaderStages)
		vkDestroyShaderModule(vkDevice, stageInfo.module, nullptr);
	m_ShaderStages.clear();
}

VkPipelineShaderStageCreateInfo Shader::CreateShaderStageInfo(const VkDevice& device, const std::string& fileName, VkShaderStageFlagBits stage) const
{
	const std::string path{ SHADER_FOLDER + fileName + EXTENSION };
	std::vector<char> shaderCode = ReadFile(path);
	VkShaderModule shaderModule = CreateShaderModule(device, shaderCode);

	VkPipelineShaderStageCreateInfo shaderStageInfo{};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = stage;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = "main"; // function to invoke
	return shaderStageInfo;
}

VkPipelineShaderStageCreateInfo Shader::CreateVertexShaderInfo(const VkDevice& device, const std::string& fileName) const
{
	return CreateShaderStageInfo(device, fileName + ".vert", VK_SHADER_STAGE_VERTEX_BIT);
}

VkPipelineShaderStageCreateInfo Shader::CreateFragmentShaderInfo(const VkDevice& device, const std::string& fileName) const
{
	return CreateShaderStageInfo(device, fileName + ".frag", VK_SHADER_STAGE_FRAGMENT_BIT);
}

VkPipelineVertexInputStateCreateInfo Shader::CreateVertexInputStateInfo() const
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	//
	//auto bindingDescription{ Vertex::GetBindingDescription() };
	//auto attributeDescription{ Vertex::GetAttributeDescriptions() };
	//
	//vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//vertexInputInfo.vertexBindingDescriptionCount = 1;
	//vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
	//vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	//vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();
	//
	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo Shader::CreateInputAssemblyStateInfo() const
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

void Shader::Start() const
{
	//VkCommandBufferBeginInfo beginInfo{};
	//beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//beginInfo.flags = 0; // Optional
	//beginInfo.pInheritanceInfo = nullptr; // Optional
	//
	//if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS)
	//	throw std::runtime_error("failed to begin recording command buffer!");
}

void Shader::Record(const std::vector<Vertex>& vertices)
{
	//VkBuffer vertexBuffers[] = { m_VertexBuffer };
	//VkDeviceSize offsets[] = { 0 };
	//vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, vertexBuffers, offsets);
	//
	//vkCmdDraw(m_CommandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
}

void Shader::End() const
{
	//if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS)
	//	throw std::runtime_error("failed to record command buffer!");
}

void Shader::Reset() const
{
	//vkResetCommandBuffer(m_CommandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
}

VkShaderModule Shader::CreateShaderModule(const VkDevice& device, const std::vector<char>& code) const
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule{};
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");

	return shaderModule;
}
