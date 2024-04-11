#include "UniformBuffer.h"

#include "vulkanbase/VulkanBase.h"
#include "vulkanbase/VulkanStructs.h"
#include "vulkanbase/VulkanUtils.h"

using namespace mk;

UniformBuffer::UniformBuffer()
{
	CreateUniformBuffers();
}

UniformBuffer::~UniformBuffer()
{
	VkDevice device{ VulkanBase::GetInstance().GetDevice() };

	for (auto buffer : m_UniformBuffers)
		vkDestroyBuffer(device, buffer, nullptr);
	m_UniformBuffers.clear();

	for (auto memory : m_UniformBuffersMemory)
		vkFreeMemory(device, memory, nullptr);
	m_UniformBuffersMemory.clear();
}

void UniformBuffer::Update(const glm::mat4& worldTransform)
{
	m_BufferInfo.model = worldTransform;
	const Camera& camera{ VulkanBase::GetInstance().GetCamera() };
	m_BufferInfo.proj = camera.GetProjectionMatrix();
	m_BufferInfo.view = camera.GetViewMatrix();

	auto image{ VulkanBase::GetInstance().GetImageIdx() };
	memcpy(m_UniformBuffersMapped[VulkanBase::GetInstance().GetSwapChain().GetCurrentFrame()], &m_BufferInfo, sizeof(m_BufferInfo));
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
