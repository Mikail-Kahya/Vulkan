#pragma once

#include <string_view>

#include "core/IComponent.h"
#include "render/IRenderer.h"

namespace mk
{
	class Pipeline3D;
}

namespace mk
{
	class Texture;
	class Mesh3D;

	class MeshComponent final : public IComponent
	{
	public:
		MeshComponent() = default;
		~MeshComponent() override;

		MeshComponent(const MeshComponent& other)				= delete;
		MeshComponent(MeshComponent&& other)					= delete;
		MeshComponent& operator=(const MeshComponent& other)	= delete;
		MeshComponent& operator=(MeshComponent&& other)			= delete;

		void SetShader(const std::string& vertex, const std::string& fragment);
		void SetMesh(const std::string& obj);
		void SetTexture(const std::string& texture) const;

		Mesh3D* GetMesh() const;
		Pipeline3D* GetPipeline() const;

	private:
		Pipeline3D* m_Pipeline{};
		Mesh3D* m_MeshPtr{};
		mesh_handle m_Handle{};
	};
}
