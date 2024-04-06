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
		int GetNrImagesViews() const;
		VkImageView GetImageView(uint32_t idx) const;
		uint32_t GetImageIdx() const;

		void Wait() const;
		void Present(uint32_t imageIdx) const;
		std::vector<VkSemaphore> GetWaitSemaphores() const;
		std::vector<VkSemaphore> GetSignalSemaphores() const;
		const VkFence& GetWaitingFence() const;

	private:
		void CreateSwapChain();
		void CreateImageViews();
		void CreateSyncObjects();

		VkSwapchainKHR m_SwapChain{ VK_NULL_HANDLE };
		VkExtent2D m_SwapChainExtent{};
		VkFormat m_SwapChainImageFormat{};
		std::vector<VkImage> m_SwapChainImages{};
		std::vector<VkImageView> m_SwapChainImageViews{};

		VkSemaphore m_ImageAvailableSemaphore{ VK_NULL_HANDLE };
		VkSemaphore m_RenderFinishedSemaphore{ VK_NULL_HANDLE };
		VkFence m_InFlightFence{ VK_NULL_HANDLE };
		
		bool m_Destroyed{ true };
	};
}