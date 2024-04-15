#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class CommandBuffer;

	class DescriptorPool final
	{
	public:
		DescriptorPool() = default;
		~DescriptorPool();

		DescriptorPool(const DescriptorPool& other)					= delete;
		DescriptorPool(DescriptorPool&& other) noexcept				= delete;
		DescriptorPool& operator=(const DescriptorPool& other)		= delete;
		DescriptorPool& operator=(DescriptorPool&& other) noexcept	= delete;

		void Initialize();
		void Destroy();

		VkDescriptorSetLayout GetLayout() const;
		VkDescriptorSetAllocateInfo GetAllocationInfo() const;

	private:
		void CreateDescriptorLayout();
		void CreateDescriptorPool();
		void CreateAllocationInfo();

		static constexpr uint32_t NR_DESCRIPTIONS{ 1000 };

		VkDescriptorSetLayout m_DescriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool m_DescriptorPool{ VK_NULL_HANDLE };
		VkDescriptorSetAllocateInfo m_AllocInfo{};
		std::vector<VkDescriptorSetLayout> m_Layouts{};
	};
}
