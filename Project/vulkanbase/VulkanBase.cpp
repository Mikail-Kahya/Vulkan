#include "VulkanBase.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <iostream>
#include <GLFW/glfw3native.h>

#include <stdexcept>

#include "VulkanUtils.h"

using namespace mk;

void VulkanBase::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void VulkanBase::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Vulkan", nullptr, nullptr);
}

void VulkanBase::InitVulkan()
{
	CreateInstance();
	SetupDebugMessenger();
	m_PhysicalDevice = PickPhysicalDevice(m_Instance);
}

void VulkanBase::MainLoop()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();
	}
}

void VulkanBase::Cleanup()
{
	if constexpr (ENABLE_VALIDATION_LAYERS)
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

	vkDestroyInstance(m_Instance, nullptr);

	glfwDestroyWindow(m_Window);
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
