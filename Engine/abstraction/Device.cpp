#include "Device.h"

#include <set>
#include <stdexcept>

#include "vulkan/VulkanUtils.h"

using namespace mk;

Device::Device(GLFWwindow* windowPtr)
{
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface(windowPtr);
	m_PhysicalDevice = PickPhysicalDevice(m_Instance, m_Surface);
	CreateLogicalDevice();
}

Device::~Device()
{
	vkDestroyDevice(m_Device, nullptr);
	if constexpr (ENABLE_VALIDATION_LAYERS)
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	vkDestroyInstance(m_Instance, nullptr);
}

Device::Device(Device&& other) noexcept
	: m_Instance{ other.m_Instance }
	, m_Surface{ other.m_Surface }
	, m_DebugMessenger{ other.m_DebugMessenger }
	, m_PhysicalDevice{ other.m_PhysicalDevice }
	, m_Device{ other.m_Device }
	, m_GraphicsQueue{ other.m_GraphicsQueue }
	, m_PresentQueue{ other.m_PresentQueue }
{
	other.m_Instance = VK_NULL_HANDLE;
	other.m_Surface = VK_NULL_HANDLE;
	other.m_DebugMessenger = VK_NULL_HANDLE;
	other.m_PhysicalDevice = VK_NULL_HANDLE;
	other.m_Device = VK_NULL_HANDLE;
	other.m_GraphicsQueue = VK_NULL_HANDLE;
	other.m_PresentQueue = VK_NULL_HANDLE;
}

Device& Device::operator=(Device&& other) noexcept
{
	m_Instance = other.m_Instance;
	m_Surface = other.m_Surface;
	m_DebugMessenger = other.m_DebugMessenger;
	m_PhysicalDevice = other.m_PhysicalDevice;
	m_Device = other.m_Device;
	m_GraphicsQueue = other.m_GraphicsQueue;
	m_PresentQueue = other.m_PresentQueue;

	other.m_Instance = VK_NULL_HANDLE;
	other.m_Surface = VK_NULL_HANDLE;
	other.m_DebugMessenger = VK_NULL_HANDLE;
	other.m_PhysicalDevice = VK_NULL_HANDLE;
	other.m_Device = VK_NULL_HANDLE;
	other.m_GraphicsQueue = VK_NULL_HANDLE;
	other.m_PresentQueue = VK_NULL_HANDLE;
	return *this;
}

void Device::Wait() const
{
	vkDeviceWaitIdle(m_Device);
}

VkDevice Device::GetDevice() const
{
	return m_Device;
}

VkPhysicalDevice Device::GetPhysicalDevice() const
{
	return m_PhysicalDevice;
}

VkSurfaceKHR Device::GetSurface() const
{
	return m_Surface;
}

VkQueue Device::GetGraphicsQueue() const
{
	return m_GraphicsQueue;
}

VkQueue Device::GetPresentQueue() const
{
	return m_PresentQueue;
}

void Device::CreateInstance()
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

void Device::SetupDebugMessenger()
{
	if constexpr (!ENABLE_VALIDATION_LAYERS)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
		throw std::runtime_error("Failed to set up debug messenger");
}

void Device::CreateSurface(GLFWwindow* windowPtr)
{
	if (glfwCreateWindowSurface(m_Instance, windowPtr, nullptr, &m_Surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface");
}

void Device::CreateLogicalDevice()
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
	deviceFeatures.samplerAnisotropy = VK_TRUE;

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
