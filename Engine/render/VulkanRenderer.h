#pragma once
#include <list>

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
		
		void Render() const override;

		int GetHeight() const noexcept;
		int GetWidth() const noexcept;

		void RegisterRender(void* renderPtr) override;
		void UnregisterRender(void* renderPtr) override;

	private:

		// Sorts by float. Whenever the float changes
		std::list<MeshComponent*> m_Renderers{};
		int m_Width{};
		int m_Height{};
	};
}

