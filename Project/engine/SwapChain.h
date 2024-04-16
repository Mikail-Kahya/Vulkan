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

		void Wait();
		void Present(uint32_t imageIdx);
		void NextFrame();

		const VkFormat& GetSwapChainImageFormat() const;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		VkViewport GetViewport() const;
		VkRect2D GetScissor() const;

		int GetNrImagesViews() const;
		VkImageView GetImageView(uint32_t idx) const;

		uint32_t GetImageIdx();
		uint32_t GetCurrentFrame() const;

		std::vector<VkSemaphore> GetWaitSemaphores() const;
		std::vector<VkSemaphore> GetSignalSemaphores() const;
		VkFence GetWaitingFence() const;

	private:
		void CreateSwapChain();
		void CreateImageViews();
		void CreateSyncObjects();

		void UpdateInternal(); // perform when external variables such as the surface change

		VkSwapchainKHR m_SwapChain{ VK_NULL_HANDLE };
		VkExtent2D m_SwapChainExtent{};
		VkFormat m_SwapChainImageFormat{};
		std::vector<VkImage> m_SwapChainImages{};
		std::vector<VkImageView> m_SwapChainImageViews{};

		std::vector<VkSemaphore> m_ImageAvailableSemaphores{};
		std::vector<VkSemaphore> m_RenderFinishedSemaphores{};
		std::vector<VkFence> m_InFlightFences{};

		uint32_t m_CurrentFrame{ 0 };
	};
}