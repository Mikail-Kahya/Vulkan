#include "VulkanBase.h"

#include "VulkanUtils.h"
#include "core/Time.h"
#include "core/ResourceManager.h"
#include "core/SceneManager.h"
#include "core/ServiceLocator.h"
#include "input/Camera.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

using namespace mk;

void VulkanBase::WindowChanged()
{
	m_FrameBufferResized = true;
}

GLFWwindow* VulkanBase::GetWindow() const
{
	return m_Window->GetWindow();
}

const Device& VulkanBase::GetDeviceAbstraction() const
{
	return *m_Device;
}

VkDevice VulkanBase::GetDevice() const
{
	return m_Device->GetDevice();
}

VkPhysicalDevice VulkanBase::GetPhysicalDevice() const
{
	return m_Device->GetPhysicalDevice();
}

VkSurfaceKHR VulkanBase::GetSurface() const
{
	return m_Device->GetSurface();
}

VkQueue VulkanBase::GetGraphicsQueue() const
{
	return m_Device->GetGraphicsQueue();
}

VkQueue VulkanBase::GetPresentQueue() const
{
	return m_Device->GetPresentQueue();
}

const RenderPass& VulkanBase::GetRenderPass() const
{
	return *m_RenderPass;
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

const DepthBuffer& VulkanBase::GetDepthBuffer() const
{
	return *m_DepthBuffer;
}

uint32_t VulkanBase::GetImageIdx() const
{
	return m_ImageIdx;
}

void VulkanBase::InitVulkan()
{
	m_Window = std::make_unique<Window>(800, 600, "Vulkan");
	m_Device = std::make_unique<Device>(m_Window->GetWindow());
	m_DescriptorPool = std::make_unique<DescriptorPool>();
	m_CommandPool = std::make_unique<CommandPool>();
	m_SwapChain = std::make_unique<SwapChain>();
	m_DepthBuffer = std::make_unique<DepthBuffer>();
	m_RenderPass = std::make_unique<RenderPass>();

	m_Camera = Camera{ static_cast<float>(m_SwapChain->GetWidth()), static_cast<float>(m_SwapChain->GetHeight()), 45.f };
	m_Mouse = Mouse{ m_Window->GetWindow() };
	m_Keyboard = Keyboard{ m_Window->GetWindow() };
}

void VulkanBase::Cleanup()
{
	m_SwapChain->Wait();
	m_Device->Wait();
	ResourceManager::Cleanup();
}

void VulkanBase::DrawFrame(const std::function<void()>& render)
{
	m_SwapChain->Wait();
	m_ImageIdx = m_SwapChain->GetImageIdx();
	if (m_FrameBufferResized)
		UpdateWindow();

	m_RenderPass->StartRecording(m_ImageIdx);
	render();
	m_RenderPass->StopRecording();
	m_SwapChain->Present(m_ImageIdx);
	m_SwapChain->NextFrame();
	
}

void VulkanBase::UpdateWindow()
{
	m_Device->Wait();
	m_FrameBufferResized = false;
	m_DepthBuffer = std::make_unique<DepthBuffer>();
	m_RenderPass->Update();
}