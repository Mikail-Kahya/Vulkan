#pragma once

#include <string_view>

#include "core/IComponent.h"

namespace mk
{
	class Texture2D;

	class MeshComponent final : public IComponent
	{
	public:
		MeshComponent(std::string vertex, std::string fragment, bool projectedOnScreen = false);
		~MeshComponent() override;

		MeshComponent(const MeshComponent& other)				= delete;
		MeshComponent(MeshComponent&& other)					= delete;
		MeshComponent& operator=(const MeshComponent& other)	= delete;
		MeshComponent& operator=(MeshComponent&& other)			= delete;

		void Start() override;

		void SetTexture(const std::string& texture);

		const std::string& GetVertexShader() const;
		const std::string& GetFragmentShader() const;
		bool IsProjected() const;


	private:
		const std::string m_VertexShader{};
		const std::string m_FragmentShader{};
		const bool m_ProjectedOnScreen{ false };
	};
}