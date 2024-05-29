#pragma once
#include <cstddef>

namespace mk
{
	class ColliderComponent;
    using collider_id = size_t;

	class IPhysicsSystem
	{
	public:
		virtual ~IPhysicsSystem() = default;

        virtual void Update()                                               = 0;
        virtual collider_id RegisterCollider(ColliderComponent* colliderPtr)= 0;
        virtual void UnregisterCollider(collider_id id)                     = 0;

	protected:
		IPhysicsSystem()											= default;
		IPhysicsSystem(const IPhysicsSystem& other)					= delete;
		IPhysicsSystem(IPhysicsSystem&& other) noexcept				= delete;
		IPhysicsSystem& operator=(const IPhysicsSystem& other)		= delete;
		IPhysicsSystem& operator=(IPhysicsSystem&& other) noexcept	= delete;
	};
}
