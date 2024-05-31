#pragma once

#include <string_view>

#include "core/IComponent.h"
#include "render/IRenderer.h"

namespace mk
{
	class Texture;
	class Mesh2D;
	class Pipeline2D;

	class SpriteComponent final : public IComponent
	{
	public:
		SpriteComponent() = default;
		~SpriteComponent() override;

		SpriteComponent(const SpriteComponent& other)				= delete;
		SpriteComponent(SpriteComponent&& other)					= delete;
		SpriteComponent& operator=(const SpriteComponent& other)	= delete;
		SpriteComponent& operator=(SpriteComponent&& other)			= delete;

		void SetShader(const std::string& vertex, const std::string& fragment);
		void SetMesh(const std::string& obj);

		Mesh2D* GetMesh() const;
		Pipeline2D* GetPipeline() const;

	private:
		Pipeline2D* m_Pipeline{};
		Mesh2D* m_MeshPtr{};
		mesh_handle m_Handle{ -1 };
	};
}
