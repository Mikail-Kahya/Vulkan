#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <functional>
#include <memory>
#include <GLFW/glfw3.h>

#include "VulkanStructs.h"
#include "engine/Camera.h"
#include "interfaces/Singleton.h"

#include "engine/CommandPool.h"
#include "engine/DescriptorPool.h"
#include "engine/Mouse.h"
#include "engine/SwapChain.h"
#include "engine/Device.h"
#include "engine/RenderPass.h"
#include "engine/Window.h"

namespace mk
{
	class VulkanBase final : public Singleton<VulkanBase>
	{
		friend class Singleton<VulkanBase>;

	public:
		virtual ~VulkanBase() override;

		void Run(const std::function<void()>& load);
		void WindowChanged();

		GLFWwindow* GetWindow() const;
		const Device& GetDeviceAbstraction() const;
		VkDevice GetDevice() const;
		VkPhysicalDevice GetPhysicalDevice() const;
		VkSurfaceKHR GetSurface() const;
		VkQueue GetGraphicsQueue() const;
		VkQueue GetPresentQueue() const;
		const RenderPass& GetRenderPass() const;;
		void WaitDrawPipeline();
		const SwapChain& GetSwapChain() const;
		const CommandPool& GetCommandPool() const;
		const Camera& GetCamera() const;
		const DescriptorPool& GetDescriptorPool() const;
		uint32_t GetImageIdx() const;

		static constexpr int MAX_FRAMES_IN_FLIGHT{ 2 };

	private:
		VulkanBase() = default;

		void InitVulkan();
		void MainLoop();
		void Cleanup();
		void DrawFrame();
		void UpdateWindow();

		// Members
		Window m_Window{ 800, 600, "Vulkan" };
		Device m_Device{ m_Window.GetWindow() };
		std::unique_ptr<DescriptorPool> m_DescriptorPool{};
		std::unique_ptr<CommandPool> m_CommandPool{};
		std::unique_ptr<SwapChain> m_SwapChain{};
		std::unique_ptr<RenderPass> m_RenderPass{};
		Camera m_Camera{};
		Mouse m_Mouse{ m_Window.GetWindow() };
		Keyboard m_Keyboard{ m_Window.GetWindow() };

		uint32_t m_ImageIdx{};

		bool m_FrameBufferResized{ false };
	};
}
