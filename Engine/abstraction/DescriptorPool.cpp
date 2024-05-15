#include "DescriptorPool.h"

#include <array>
#include <stdexcept>

#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

DescriptorPool::DescriptorPool()
{
	CreateDescriptorLayout();
	CreateDescriptorPool();
	CreateAllocationInfo();
}

DescriptorPool::~DescriptorPool()
{
	const VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	if (m_DescriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(device, m_DescriptorPool, nullptr);
		m_DescriptorPool = VK_NULL_HANDLE;
	}

	if (m_DescriptorSetLayout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
		m_DescriptorSetLayout = VK_NULL_HANDLE;
	}
}

VkDescriptorSetLayout DescriptorPool::GetLayout() const
{
	return m_DescriptorSetLayout;
}

VkDescriptorSetAllocateInfo DescriptorPool::GetAllocationInfo() const
{
	return m_AllocInfo;
}

VkDescriptorPool DescriptorPool::GetVkDescriptorPool() const
{
	return m_DescriptorPool;
}

void DescriptorPool::CreateDescriptorLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional

	const std::array<VkDescriptorSetLayoutBinding, 2> bindings{ uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(VulkanBase::GetInstance().GetDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		throw std::runtime_error("Failed to create descriptor set layout");
}

void DescriptorPool::CreateDescriptorPool()
{
	constexpr uint32_t nrSets{ static_cast<uint32_t>(VulkanBase::MAX_FRAMES_IN_FLIGHT) * NR_DESCRIPTIONS };

	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = nrSets;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = nrSets;

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = nrSets * 2;

	if (vkCreateDescriptorPool(VulkanBase::GetInstance().GetDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		throw std::runtime_error("Failed to create descriptor pool");
}

void DescriptorPool::CreateAllocationInfo()
{
	m_Layouts.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	for (auto& layout : m_Layouts)
		layout = m_DescriptorSetLayout;

	m_AllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	m_AllocInfo.descriptorPool = m_DescriptorPool;
	m_AllocInfo.descriptorSetCount = static_cast<uint32_t>(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	m_AllocInfo.pSetLayouts = m_Layouts.data();
}
