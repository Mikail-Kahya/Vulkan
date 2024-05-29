#pragma once

#include "core/IComponent.h"

namespace mk
{
	class ColliderComponent final : public IComponent
	{
	public: 
		ColliderComponent();
		~ColliderComponent() override;

		ColliderComponent(const ColliderComponent& other)				= delete;
		ColliderComponent(ColliderComponent&& other)					= delete;
		ColliderComponent& operator=(const ColliderComponent& other)	= delete;
		ColliderComponent& operator=(ColliderComponent&& other)			= delete;
	private:

	};
}
