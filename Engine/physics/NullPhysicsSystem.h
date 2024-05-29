#pragma once
#include "IPhysicsSystem.h"

namespace mk
{
	class ColliderComponent;

	class NullPhysicsSystem final : public IPhysicsSystem
	{
	public:
		NullPhysicsSystem() = default;
		virtual ~NullPhysicsSystem() = default;

		void Update() override {};
        collider_id RegisterCollider(ColliderComponent*) override { return 0; }
        void UnregisterCollider(collider_id) override {}

	private:
		NullPhysicsSystem(const NullPhysicsSystem& other)					= delete;
		NullPhysicsSystem(NullPhysicsSystem&& other) noexcept				= delete;
		NullPhysicsSystem& operator=(const NullPhysicsSystem& other)		= delete;
		NullPhysicsSystem& operator=(NullPhysicsSystem&& other) noexcept	= delete;
	};
}
