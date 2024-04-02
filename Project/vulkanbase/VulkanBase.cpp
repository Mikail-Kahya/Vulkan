#include "VulkanBase.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <iostream>
#include <set>
#include <GLFW/glfw3native.h>

#include <stdexcept>

#include "VulkanUtils.h"

#include <vector>

using namespace mk;

void VulkanBase::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

GLFWwindow* VulkanBase::GetWindow() const
{
	return m_WindowPtr;
}

const VkDevice& VulkanBase::GetDevice() const
{
	return m_Device;
}

const VkPhysicalDevice& VulkanBase::GetPhysicalDevice() const
{
	return m_PhysicalDevice;
}

const VkSurfaceKHR& VulkanBase::GetSurface() const
{
	return m_Surface;
}

const SwapChain& VulkanBase::GetSwapChain() const
{
	return m_SwapChain;
}

void VulkanBase::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_WindowPtr = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Vulkan", nullptr, nullptr);
}

void VulkanBase::InitVulkan()
{
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	m_PhysicalDevice = PickPhysicalDevice(m_Instance, m_Surface);
	CreateLogicalDevice();
	m_SwapChain.Initialize();
	m_Pipeline.Initialize("shader");
}

void VulkanBase::MainLoop()
{
	while (!glfwWindowShouldClose(m_WindowPtr))
	{
		glfwPollEvents();
	}
}

void VulkanBase::Cleanup()
{
	m_Pipeline.Destroy();
	m_SwapChain.Destroy();
	
	vkDestroyDevice(m_Device, nullptr);
	if constexpr (ENABLE_VALIDATION_LAYERS)
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	vkDestroyInstance(m_Instance, nullptr);

	glfwDestroyWindow(m_WindowPtr);
	glfwTerminate();
}

void VulkanBase::CreateInstance()
{
	if (ENABLE_VALIDATION_LAYERS && !CheckValidationLayerSupport())
		throw std::runtime_error("Validation layers requested, but not available");

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan tutorial";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "MKUltra";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	const auto extensions{ GetRequiredExtensions() };
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if constexpr (ENABLE_VALIDATION_LAYERS)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}
		

	if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance");
}

void VulkanBase::SetupDebugMessenger()
{
	if constexpr (!ENABLE_VALIDATION_LAYERS) 
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
		throw std::runtime_error("Failed to set up debug messenger");
}

void VulkanBase::CreateLogicalDevice()
{
	const QueueFamilyIndices indices{ FindQueueFamilies(m_PhysicalDevice, m_Surface) };
	constexpr float queuePriority{ 1.0f };

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	std::set<uint32_t> uniqueQueueFamilies{ indices.graphicsFamily.value(), indices.presentFamily.value() };

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		// Get queue info
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Specify physical device features
	VkPhysicalDeviceFeatures deviceFeatures{};

	// Create device
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
	createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

	if constexpr (ENABLE_VALIDATION_LAYERS)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else
		createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device");

	vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
}

void VulkanBase::CreateSurface()
{
	if (glfwCreateWindowSurface(m_Instance, m_WindowPtr, nullptr, &m_Surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface");
}