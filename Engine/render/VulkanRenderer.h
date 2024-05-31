#pragma once
#include <memory>
#include <vector>

#include "IRenderer.h"

namespace mk
{
	class MeshComponent;
    class CameraComponent;

	class VulkanRenderer final : public IRenderer
	{
	public:
		VulkanRenderer(int width, int height);
		~VulkanRenderer() override;

		VulkanRenderer(const VulkanRenderer& other)					= delete;
		VulkanRenderer(VulkanRenderer&& other) noexcept				= delete;
		VulkanRenderer& operator=(const VulkanRenderer& other)		= delete;
		VulkanRenderer& operator=(VulkanRenderer&& other) noexcept	= delete;
		
		void Render() const override;
		void Update() override;

		int GetHeight() const noexcept;
		int GetWidth() const noexcept;

		mesh_handle RegisterMeshRender(MeshComponent* meshCompPtr) override;
		mesh_handle RegisterSpriteRender(SpriteComponent* objectPtr) override;
		void UnregisterMeshRender(mesh_handle handle) override;
		void UnregisterSpriteRender(mesh_handle handle) override;

        void RegisterCamera(CameraComponent* cameraPtr) override;
        void UnregisterCamera(CameraComponent* cameraPtr) override;
        void SetActiveCamera(CameraComponent* cameraPtr) override;

	private:
		class VulkanImpl;
		using Impl = std::unique_ptr<VulkanImpl>;

		std::vector<MeshComponent*> m_Meshes{};
		std::vector<SpriteComponent*> m_Sprites{};
        std::vector<CameraComponent*> m_Cameras{};
        CameraComponent* m_RenderCamera{};
		Impl m_Impl;
	};
}

