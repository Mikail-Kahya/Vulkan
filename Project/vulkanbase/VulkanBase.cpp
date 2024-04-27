#include "VulkanBase.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <set>
#include <stdexcept>
#include <vector>

#include "VulkanUtils.h"
#include "engine/ResourceManager.h"
#include "engine/SceneManager.h"
#include "engine/Time.h"

using namespace mk;

VulkanBase::~VulkanBase()
{
	Cleanup();
}

void VulkanBase::Run(const std::function<void()>& load)
{
	InitVulkan();
	Time::Update();
	load();
	MainLoop();
	Cleanup();
}

void VulkanBase::WindowChanged()
{
	m_FrameBufferResized = true;
}

GLFWwindow* VulkanBase::GetWindow() const
{
	return m_Window.GetWindow();
}

const Device& VulkanBase::GetDeviceAbstraction() const
{
	return m_Device;
}

VkDevice VulkanBase::GetDevice() const
{
	return m_Device.GetDevice();
}

VkPhysicalDevice VulkanBase::GetPhysicalDevice() const
{
	return m_Device.GetPhysicalDevice();
}

VkSurfaceKHR VulkanBase::GetSurface() const
{
	return m_Device.GetSurface();
}

VkQueue VulkanBase::GetGraphicsQueue() const
{
	return m_Device.GetGraphicsQueue();
}

VkQueue VulkanBase::GetPresentQueue() const
{
	return m_Device.GetPresentQueue();
}

const RenderPass& VulkanBase::GetRenderPass() const
{
	return *m_RenderPass;
}

void VulkanBase::WaitDrawPipeline()
{
	m_SwapChain->Wait();
}

const SwapChain& VulkanBase::GetSwapChain() const
{
	return *m_SwapChain;
}

const CommandPool& VulkanBase::GetCommandPool() const
{
	return *m_CommandPool;
}

const Camera& VulkanBase::GetCamera() const
{
	return m_Camera;
}

const DescriptorPool& VulkanBase::GetDescriptorPool() const
{
	return *m_DescriptorPool;
}

uint32_t VulkanBase::GetImageIdx() const
{
	return m_ImageIdx;
}

void VulkanBase::InitVulkan()
{
	m_DescriptorPool = std::make_unique<DescriptorPool>();
	m_CommandPool = std::make_unique<CommandPool>();
	m_SwapChain = std::make_unique<SwapChain>();
	m_RenderPass = std::make_unique<RenderPass>();

	m_Camera = Camera{ static_cast<float>(m_SwapChain->GetWidth()), static_cast<float>(m_SwapChain->GetHeight()), 45.f };
}

void VulkanBase::MainLoop()
{
	//glfwPollEvents();
	//m_Camera.Update();
	//DrawFrame();
	//return;
	while (!m_Window.ShouldClose())
	{
		Time::Update();
		glfwPollEvents();
		m_Mouse.Update();
		m_Camera.Update(m_Mouse, m_Keyboard);
		DrawFrame();
	}

	m_Device.Wait();
}

void VulkanBase::Cleanup()
{
	SceneManager::GetInstance().Cleanup();
	ResourceManager::GetInstance().Cleanup();
}

void VulkanBase::DrawFrame()
{
	m_SwapChain->Wait();
	m_ImageIdx = m_SwapChain->GetImageIdx();
	if (m_FrameBufferResized)
		UpdateWindow();

	m_RenderPass->StartRecording(m_ImageIdx);
	SceneManager::GetInstance().Draw();
	m_RenderPass->StopRecording();
	m_SwapChain->Present(m_ImageIdx);
	m_SwapChain->NextFrame();
	
}

void VulkanBase::UpdateWindow()
{
	m_Device.Wait();
	m_FrameBufferResized = false;
	m_RenderPass->Update();
	ResourceManager::GetInstance().Update();
}