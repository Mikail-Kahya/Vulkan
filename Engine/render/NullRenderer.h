#pragma once
#include "IRenderer.h"

namespace mk
{
	class NullRenderer final : public IRenderer
	{
	public:
		virtual ~NullRenderer() = default;

		virtual void Render() const {}
		virtual void RegisterRender(void* objectPtr) {}
		virtual void UnregisterRender(void* objectPtr) {}

	protected:
		NullRenderer() = default;
		NullRenderer(const NullRenderer& other)					= delete;
		NullRenderer(NullRenderer&& other) noexcept				= delete;
		NullRenderer& operator=(const NullRenderer& other)		= delete;
		NullRenderer& operator=(NullRenderer&& other) noexcept	= delete;
	};
}
