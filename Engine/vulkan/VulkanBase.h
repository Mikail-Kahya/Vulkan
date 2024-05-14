#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <functional>
#include <memory>
#include <GLFW/glfw3.h>

#include "VulkanStructs.h"
#include "input/Camera.h"
#include "input/Mouse.h"
#include "CRTP/ISingleton.h"

#include "abstraction/CommandPool.h"
#include "abstraction/DepthBuffer.h"
#include "abstraction/DescriptorPool.h"
#include "abstraction/SwapChain.h"
#include "abstraction/Device.h"
#include "abstraction/RenderPass.h"
#include "abstraction/Window.h"

namespace mk
{
	class DepthBuffer;
}

namespace mk
{
	class VulkanBase final : public ISingleton<VulkanBase>
	{
		friend class ISingleton<VulkanBase>;

	public:
		virtual ~VulkanBase() override = default;

		void InitVulkan();
		void DrawFrame();
		void Cleanup();
		void WindowChanged();

		GLFWwindow* GetWindow() const;
		const Device& GetDeviceAbstraction() const;
		VkDevice GetDevice() const;
		VkPhysicalDevice GetPhysicalDevice() const;
		VkSurfaceKHR GetSurface() const;
		VkQueue GetGraphicsQueue() const;
		VkQueue GetPresentQueue() const;
		const RenderPass& GetRenderPass() const;;
		const SwapChain& GetSwapChain() const;
		const CommandPool& GetCommandPool() const;
		const Camera& GetCamera() const;
		const DescriptorPool& GetDescriptorPool() const;
		const DepthBuffer& GetDepthBuffer() const;
		uint32_t GetImageIdx() const;

		static constexpr int MAX_FRAMES_IN_FLIGHT{ 2 };

	private:
		VulkanBase() = default;

		void UpdateWindow();

		// Members
		std::unique_ptr<Window> m_Window{};
		std::unique_ptr<Device> m_Device{};
		std::unique_ptr<DescriptorPool> m_DescriptorPool{};
		std::unique_ptr<CommandPool> m_CommandPool{};
		std::unique_ptr<SwapChain> m_SwapChain{};
		std::unique_ptr<DepthBuffer> m_DepthBuffer{};
		std::unique_ptr<RenderPass> m_RenderPass{};
		Camera m_Camera{};
		Mouse m_Mouse{ nullptr };
		Keyboard m_Keyboard{ nullptr };

		uint32_t m_ImageIdx{};

		bool m_FrameBufferResized{ false };
	};
}
