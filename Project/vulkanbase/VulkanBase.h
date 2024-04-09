#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

#include "VulkanStructs.h"
#include "interfaces/Singleton.h"

#include "engine/CommandPool.h"
#include "engine/Pipeline.h"
#include "engine/SwapChain.h"

namespace mk
{
	class VulkanBase final : public Singleton<VulkanBase>
	{
	public:
		VulkanBase() = default;
		void Run();
		void WindowChanged();

		GLFWwindow* GetWindow() const;
		VkDevice GetDevice() const;
		VkPhysicalDevice GetPhysicalDevice() const;
		VkSurfaceKHR GetSurface() const;
		VkQueue GetGraphicsQueue() const;
		VkQueue GetPresentQueue() const;
		const SwapChain& GetSwapChain() const;
		const CommandPool& GetCommandPool() const;

		static constexpr int MAX_FRAMES_IN_FLIGHT{ 2 };

	private:
		void InitWindow();
		void InitVulkan();
		void InitScenes();
		void MainLoop();
		void Cleanup();
		void DrawFrame();
		void UpdateWindow();

		// Vulkan initialization
		void CreateInstance(); // Links device to library
		void SetupDebugMessenger();
		void CreateLogicalDevice(); // Interacts between physical device and queue
		void CreateSurface();

		// STATICS
		static constexpr uint32_t SCREEN_WIDTH{ 800 };
		static constexpr uint32_t SCREEN_HEIGHT{ 600 };

		// Members
		GLFWwindow* m_WindowPtr{ nullptr };

		VkInstance m_Instance{ VK_NULL_HANDLE }; // interface with device
		VkSurfaceKHR m_Surface{ VK_NULL_HANDLE }; // interface with glfw window
		VkDebugUtilsMessengerEXT m_DebugMessenger{ VK_NULL_HANDLE }; // debugging
		VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE }; // Graphics card used

		VkDevice m_Device{ VK_NULL_HANDLE }; // Logical device interaction between physical and queues
		VkQueue m_GraphicsQueue{ VK_NULL_HANDLE };
		VkQueue m_PresentQueue{ VK_NULL_HANDLE };

		Pipeline m_Pipeline{};
		SwapChain m_SwapChain{};
		CommandPool m_CommandPool{};

		bool m_FrameBufferResized{ false };
	};
}
