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

	template <typename Vertex>
	concept IsVertex = requires()
	{
		{ Vertex::GetAttributeDescriptions() } -> std::same_as<std::vector<VkVertexInputAttributeDescription>*>;
		{ Vertex::GetBindingDescription() } -> std::same_as <VkVertexInputBindingDescription*>;
	};


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
	VkFormat FindSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat(VkPhysicalDevice physicalDevice);
	bool HasStencilComponent(VkFormat format);

	std::vector<char> ReadFile(const std::string& fileName);
	void FrameBufferResizeCallback(GLFWwindow* windowPtr, int width, int height);

	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void CreateBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkImageView CreateVkImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlag);
	void CreateImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image_out, VkDeviceMemory& imageMemory_out);
	void CreateVkImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage& image_out);
	void CreateVkMemory(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties, VkImage image, VkDeviceMemory& imageMemory_out);

	// Pipeline creation
	VkPipelineDynamicStateCreateInfo CreateDynamicState(const std::vector<VkDynamicState>& dynamicStates);
	
	template <IsVertex Vertex>
	VkPipelineVertexInputStateCreateInfo CreateVertexInfo()
	{
		const auto attributeDescriptions = Vertex::GetAttributeDescriptions();
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = Vertex::GetBindingDescription();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions->size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions->data();
		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo CreateInputAssembly();
	VkPipelineViewportStateCreateInfo CreateViewportState();
	VkPipelineRasterizationStateCreateInfo CreateRasterizer();
	VkPipelineMultisampleStateCreateInfo CreateMultisampling();
	VkPipelineColorBlendAttachmentState CreateColorBlendAttachment();
	VkPipelineColorBlendStateCreateInfo CreateColorBlend(VkPipelineColorBlendAttachmentState* colorBlendAttachment);
	VkPipelineDepthStencilStateCreateInfo CreateDepthStencilInfo();
}