#pragma once
#include <future>
#include <string>

namespace mk
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Render() const						= 0;
		virtual void RegisterRender(void* objectPtr)	= 0;
		virtual void UnregisterRender(void* objectPtr)	= 0;

	protected:
		IRenderer() = default;
		IRenderer(const IRenderer& other)					= delete;
		IRenderer(IRenderer&& other) noexcept				= delete;
		IRenderer& operator=(const IRenderer& other)		= delete;
		IRenderer& operator=(IRenderer&& other) noexcept	= delete;
	};
}
