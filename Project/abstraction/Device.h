#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <functional>
#include <GLFW/glfw3.h>

#include "vulkan/VulkanStructs.h"

namespace mk
{
	class Device final
	{
	public:
		Device(GLFWwindow* windowPtr);
		~Device();

		Device(const Device& other)					= delete;
		Device(Device&& other) noexcept;
		Device& operator=(const Device& other)		= delete;
		Device& operator=(Device&& other) noexcept;

		void Wait() const;

		VkDevice GetDevice() const;
		VkPhysicalDevice GetPhysicalDevice() const;
		VkSurfaceKHR GetSurface() const;
		VkQueue GetGraphicsQueue() const;
		VkQueue GetPresentQueue() const;

		static constexpr int MAX_FRAMES_IN_FLIGHT{ 2 };

	private:
		void CreateInstance(); // Links device to library
		void SetupDebugMessenger();
		void CreateSurface(GLFWwindow* windowPtr);
		void CreateLogicalDevice(); // Interacts between physical device and queue

		VkInstance m_Instance{ VK_NULL_HANDLE }; // interface with device
		VkSurfaceKHR m_Surface{ VK_NULL_HANDLE }; // interface with glfw window
		VkDebugUtilsMessengerEXT m_DebugMessenger{ VK_NULL_HANDLE }; // debugging
		VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE }; // Graphics card used

		VkDevice m_Device{ VK_NULL_HANDLE }; // Logical device interaction between physical and queues
		VkQueue m_GraphicsQueue{ VK_NULL_HANDLE };
		VkQueue m_PresentQueue{ VK_NULL_HANDLE };
	};
}
