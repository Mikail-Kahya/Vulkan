#pragma once
#include "stdint.h"

namespace mk
{
	using mesh_handle = int32_t;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Render() const	= 0;
		virtual void Update()		= 0;
		virtual mesh_handle RegisterRender(void* objectPtr)	= 0;
		virtual void UnregisterRender(mesh_handle handle)	= 0;

	protected:
		IRenderer() = default;
		IRenderer(const IRenderer& other)					= delete;
		IRenderer(IRenderer&& other) noexcept				= delete;
		IRenderer& operator=(const IRenderer& other)		= delete;
		IRenderer& operator=(IRenderer&& other) noexcept	= delete;
	};
}
