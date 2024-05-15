#pragma once
#include <memory>
#include <vector>

#include "IRenderer.h"

namespace mk
{
	class MeshComponent;

	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class VulkanRenderer final : public IRenderer
	{
	public:
		VulkanRenderer() = default;
		VulkanRenderer(int width, int height);
		~VulkanRenderer() override = default;

		VulkanRenderer(const VulkanRenderer& other)					= delete;
		VulkanRenderer(VulkanRenderer&& other) noexcept				= delete;
		VulkanRenderer& operator=(const VulkanRenderer& other)		= delete;
		VulkanRenderer& operator=(VulkanRenderer&& other) noexcept	= delete;
		
		void Render() const override;
		void Update() override {}

		int GetHeight() const noexcept;
		int GetWidth() const noexcept;

		mesh_handle RegisterRender(void* objectPtr) override;
		void UnregisterRender(mesh_handle handle) override;

	private:
		class VulkanImpl;
		std::unique_ptr<VulkanImpl> m_Impl;
		std::vector<MeshComponent*> m_Renderers{};
		int m_Width{};
		int m_Height{};
	};
}

