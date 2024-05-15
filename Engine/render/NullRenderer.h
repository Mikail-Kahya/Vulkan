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
		mesh_handle RegisterRender(void*) override { return 0; }
		void UnregisterRender(mesh_handle) override {}
	};
}
