#pragma once
#include "stdint.h"

namespace mk
{
    class CameraComponent;
	class MeshComponent;
	class SpriteComponent;
	using mesh_handle = int32_t;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Render() const	= 0;
		virtual void Update()		= 0;
		virtual mesh_handle RegisterMeshRender(MeshComponent* objectPtr) = 0;
		virtual mesh_handle RegisterSpriteRender(SpriteComponent* objectPtr) = 0;
		virtual void UnregisterMeshRender(mesh_handle handle) = 0;
		virtual void UnregisterSpriteRender(mesh_handle handle) = 0;

        virtual void RegisterCamera(CameraComponent* cameraPtr)     = 0;
        virtual void UnregisterCamera(CameraComponent* cameraPtr)   = 0;
        virtual void SetActiveCamera(CameraComponent* cameraPtr)    = 0;

	protected:
		IRenderer() = default;
		IRenderer(const IRenderer& other)					= delete;
		IRenderer(IRenderer&& other) noexcept				= delete;
		IRenderer& operator=(const IRenderer& other)		= delete;
		IRenderer& operator=(IRenderer&& other) noexcept	= delete;
	};
}
