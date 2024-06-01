#pragma once
#include <memory>
#include <unordered_map>

#include "IComponent.h"
#include "IState.h"

namespace mk
{
	class StateComponent : public IComponent
	{
	public:
		StateComponent(const std::string& stateId, std::unique_ptr<IState>&& startState);
		~StateComponent() override = default;

		StateComponent(const StateComponent& other)					= delete;
		StateComponent(StateComponent&& other) noexcept				= delete;
		StateComponent& operator=(const StateComponent& other)		= delete;
		StateComponent& operator=(StateComponent&& other) noexcept	= delete;

		void Start() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		bool AddState(std::string stateId, std::unique_ptr<IState>&& state);

	private:
		void SwitchState(const std::string& stateId);

		std::unordered_map<std::string, std::unique_ptr<IState>> m_States{};
		IState* m_CurrentState{};
	};
}
