#pragma once
#include "IState.h"

namespace mk
{
	class GameObject;

	class GameObjectState : public IState
	{
		friend class StateComponent;

	public:
		~GameObjectState() override = default;

		GameObjectState(const GameObjectState& other)					= delete;
		GameObjectState(GameObjectState&& other) noexcept				= delete;
		GameObjectState& operator=(const GameObjectState& other)		= delete;
		GameObjectState& operator=(GameObjectState&& other) noexcept	= delete;

		virtual void Start() {}

	protected:
		GameObjectState() = default;
		GameObject* GetOwner() const { return m_OwnerPtr; }

	private:
		void SetOwner(GameObject* ownerPtr) { m_OwnerPtr = ownerPtr; }

		GameObject* m_OwnerPtr{};
	};
}
