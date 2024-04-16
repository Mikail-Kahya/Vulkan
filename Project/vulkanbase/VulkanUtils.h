#pragma once
#include <vector>
#include <string>

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include "VulkanStructs.h"
#include "GLFW/glfw3.h"

namespace mk
{
	inline const std::vector<const char*> VALIDATION_LAYERS{
			"VK_LAYER_KHRONOS_validation"
	};

	inline const std::vector<const char*> DEVICE_EXTENSIONS{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
#ifdef NDEBUG
	inline constexpr bool ENABLE_VALIDATION_LAYERS{ false };
#else
	inline constexpr bool ENABLE_VALIDATION_LAYERS{ true };
#endif



	// Functions
	bool CheckValidationLayerSupport();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	std::vector<const char*> GetRequiredExtensions();
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackDataPtr,
		void* userDataPtr);
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* createInfoPtr,
		const VkAllocationCallbacks* allocatorPtr,
		VkDebugUtilsMessengerEXT* debugMessengerPtr);
	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	[[nodiscard]] VkPhysicalDevice PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	uint32_t RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* windowPtr);

	std::vector<char> ReadFile(const std::string& fileName);
	void FrameBufferResizeCallback(GLFWwindow* windowPtr, int width, int height);

	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void CreateBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
}