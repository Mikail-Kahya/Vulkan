#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

#include "VulkanStructs.h"

namespace mk
{
	class VulkanBase final {
	public:
		VulkanBase() = default;
		void Run();

	private:
		void InitWindow();
		void InitVulkan();
		void MainLoop();
		void Cleanup();

		// Vulkan initialization
		void CreateInstance(); // Links device to library
		void SetupDebugMessenger();

		// STATICS
		static constexpr uint32_t SCREEN_WIDTH{ 800 };
		static constexpr uint32_t SCREEN_HEIGHT{ 600 };

		// Members
		GLFWwindow* m_Window{ nullptr };

		VkInstance m_Instance{ VK_NULL_HANDLE };
		VkDebugUtilsMessengerEXT m_DebugMessenger{ VK_NULL_HANDLE };
		VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE };
		VkDevice m_Device{ VK_NULL_HANDLE };
	};
}