#pragma once
namespace mk
{
	using mesh_handle = size_t;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Render() const	= 0;
		virtual void Update() const	= 0;
		virtual mesh_handle RegisterRender(void* objectPtr)	= 0;
		virtual void UnregisterRender(mesh_handle handle)	= 0;
		virtual void SetTexture(mesh_handle handle, const std::string& path)	= 0;
		virtual void SetMesh(mesh_handle handle, const std::string& path)		= 0;

	protected:
		IRenderer() = default;
		IRenderer(const IRenderer& other)					= delete;
		IRenderer(IRenderer&& other) noexcept				= delete;
		IRenderer& operator=(const IRenderer& other)		= delete;
		IRenderer& operator=(IRenderer&& other) noexcept	= delete;
	};
}
