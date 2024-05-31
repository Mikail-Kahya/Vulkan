#pragma once
#include "IRenderer.h"

namespace mk
{
	class NullRenderer final : public IRenderer
	{
	public:
		NullRenderer() = default;
		~NullRenderer() override = default;

		NullRenderer(const NullRenderer& other)					= delete;
		NullRenderer(NullRenderer&& other) noexcept				= delete;
		NullRenderer& operator=(const NullRenderer& other)		= delete;
		NullRenderer& operator=(NullRenderer&& other) noexcept	= delete;

		void Render() const override {}
		void Update() override {}
		mesh_handle RegisterMeshRender(MeshComponent*) override { return 0; }
		mesh_handle RegisterSpriteRender(SpriteComponent*) override { return 0; }
		void UnregisterMeshRender(mesh_handle) override {}
		void UnregisterSpriteRender(mesh_handle) override {}
        void RegisterCamera(CameraComponent*) override {}
        void UnregisterCamera(CameraComponent*) override {}
        void SetActiveCamera(CameraComponent*) override {}
	};
}
