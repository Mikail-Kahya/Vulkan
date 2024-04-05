#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace mk
{
	class SwapChain final
	{
	public:
		SwapChain() = default;
		~SwapChain();

		SwapChain(const SwapChain& other) = delete;
		SwapChain(SwapChain&& other) noexcept = delete;
		SwapChain& operator=(const SwapChain& other) = delete;
		SwapChain& operator=(SwapChain&& other) noexcept = delete;

		void Initialize();
		void Destroy();

		const VkFormat& GetSwapChainImageFormat() const;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		VkViewport GetViewport() const;
		VkRect2D GetScissor() const;
		int GetNrImages() const;
		VkImageView GetImage(uint32_t idx) const;

	private:
		void CreateSwapChain();
		void CreateImageViews();

		VkSwapchainKHR m_SwapChain{ VK_NULL_HANDLE };
		VkExtent2D m_SwapChainExtent{};
		VkFormat m_SwapChainImageFormat{};
		std::vector<VkImage> m_SwapChainImages{};
		std::vector<VkImageView> m_SwapChainImageViews{};
		
		bool m_Destroyed{ true };
	};
}