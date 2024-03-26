#pragma once
#include <vector>
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
}


/*
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32

#include <set>
#include <vector>
#include <fstream>

#include <GLFW/glfw3native.h>


#ifdef NDEBUG
constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> DEVICE_EXTENSIONS{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

std::vector<char> ReadFile(const std::string& filename);

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface, VkQueueFlagBits queueFlagBit = VK_QUEUE_GRAPHICS_BIT);

uint32_t FindMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

std::vector<const char*> GetRequiredExtensions();

bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);

bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* windowPtr);

VkCommandBuffer CreateCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
*/