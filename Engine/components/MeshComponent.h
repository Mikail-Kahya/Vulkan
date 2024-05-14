#pragma once

#include <string_view>

#include "core/IComponent.h"
#include "core/Transform.h"

namespace mk
{
	class Texture2D;

	class MeshComponent final : public IComponent
	{
	public:
		MeshComponent() = default;
		~MeshComponent() override;

		MeshComponent(const MeshComponent& other)				= delete;
		MeshComponent(MeshComponent&& other)					= delete;
		MeshComponent& operator=(const MeshComponent& other)	= delete;
		MeshComponent& operator=(MeshComponent&& other)			= delete;

		void Start() override;

	private:
	};
}