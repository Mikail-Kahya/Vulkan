#pragma once
#include <memory>
#include "IPhysicsSystem.h"

namespace mk
{
	class JoltPhysicsSystem final : public IPhysicsSystem
	{
	public:
        JoltPhysicsSystem(int32_t maxBodies = 1024);
		~JoltPhysicsSystem() override = default;
		JoltPhysicsSystem(const JoltPhysicsSystem& other)					= delete;
		JoltPhysicsSystem(JoltPhysicsSystem&& other) noexcept				= delete;
		JoltPhysicsSystem& operator=(const JoltPhysicsSystem& other)		= delete;
		JoltPhysicsSystem& operator=(JoltPhysicsSystem&& other) noexcept	= delete;
		

		void Update() override;
        collider_id RegisterCollider(ColliderComponent* colliderPtr) override;
        void UnregisterCollider(collider_id id) override;

    private:
        class JoltPhysicsImpl;
        std::unique_ptr<JoltPhysicsImpl> m_Physics;
	};
}
