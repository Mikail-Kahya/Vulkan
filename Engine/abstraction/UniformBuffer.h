#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

#include "vulkan/VulkanStructs.h"

namespace mk
{
	class Texture;

	class UniformBuffer final
	{
	public:
		UniformBuffer(const Texture* texture);
		~UniformBuffer();

		UniformBuffer(const UniformBuffer& other)					= delete;
		UniformBuffer(UniformBuffer&& other) noexcept				= delete;
		UniformBuffer& operator=(const UniformBuffer& other)		= delete;
		UniformBuffer& operator=(UniformBuffer&& other) noexcept	= delete;

		void Update(const glm::mat4& worldTransform);
		void SetActive(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const;

	private:
		void CreateUniformBuffers();
		void CreateDescriptorSets(const Texture* texture);

		UniformBufferObject m_BufferInfo{};

		std::vector<VkBuffer> m_UniformBuffers{};
		std::vector<VkDeviceMemory> m_UniformBuffersMemory{};
		std::vector<void*> m_UniformBuffersMapped{};
		std::vector<VkDescriptorSet> m_DescriptorSets{};
	};
}
