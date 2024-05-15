#include "UniformBuffer.h"

#include <array>
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "vulkan/VulkanBase.h"
#include "vulkan/VulkanStructs.h"
#include "vulkan/VulkanUtils.h"

using namespace mk;

UniformBuffer::UniformBuffer(const Texture* texture)
{
	CreateUniformBuffers();
	CreateDescriptorSets(texture);
}

UniformBuffer::~UniformBuffer()
{
	const VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	for (auto memory : m_UniformBuffersMemory)
		vkFreeMemory(device, memory, nullptr);

	for (auto buffer : m_UniformBuffers)
		vkDestroyBuffer(device, buffer, nullptr);
		
	m_UniformBuffers.clear();

	
	m_UniformBuffersMemory.clear();
}

void UniformBuffer::Update(const glm::mat4& worldTransform)
{
	const Camera& camera{ VulkanBase::GetInstance().GetCamera() };
	m_BufferInfo.model = worldTransform;
	m_BufferInfo.proj = camera.GetProjectionMatrix();
	m_BufferInfo.view = camera.GetViewMatrix();
	memcpy(m_UniformBuffersMapped[VulkanBase::GetInstance().GetSwapChain().GetCurrentFrame()], &m_BufferInfo, sizeof(m_BufferInfo));
}

void UniformBuffer::SetActive(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const
{
	vkCmdBindDescriptorSets(
		commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 
		1, &m_DescriptorSets[VulkanBase::GetInstance().GetSwapChain().GetCurrentFrame()], 
		0, nullptr);
}

void UniformBuffer::CreateUniformBuffers()
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	VkDevice device{ vulkanBase.GetDevice() };
	VkPhysicalDevice physicalDevice{ vulkanBase.GetPhysicalDevice() };

	VkDeviceSize bufferSize{ sizeof(UniformBufferObject) };

	m_UniformBuffers.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	m_UniformBuffersMemory.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	m_UniformBuffersMapped.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);

	for (size_t idx{}; idx < VulkanBase::MAX_FRAMES_IN_FLIGHT; ++idx) 
	{
		CreateBuffer(	device, physicalDevice, bufferSize,
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
						m_UniformBuffers[idx], m_UniformBuffersMemory[idx]);

		vkMapMemory(device, m_UniformBuffersMemory[idx], 0, bufferSize, 0, &m_UniformBuffersMapped[idx]);
	}
}

void UniformBuffer::CreateDescriptorSets(const Texture* texture)
{
	const VulkanBase& vulkanBase{ VulkanBase::GetInstance() };
	const VkDevice device{ vulkanBase.GetDevice() };
	const auto allocInfo{ vulkanBase.GetDescriptorPool().GetAllocationInfo() };

	const auto imageInfo{ texture->GetImageInfo() };
	m_DescriptorSets.resize(VulkanBase::MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(VulkanBase::GetInstance().GetDevice(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate descriptor sets");

	for (size_t idx{}; idx < VulkanBase::MAX_FRAMES_IN_FLIGHT; ++idx)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_UniformBuffers[idx];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_DescriptorSets[idx];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pImageInfo = nullptr; // Optional
		descriptorWrites[0].pTexelBufferView = nullptr; // Optional

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = m_DescriptorSets[idx];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;
		descriptorWrites[1].pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

}
