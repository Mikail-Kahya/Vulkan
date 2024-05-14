#include "StateComponent.h"

#include "DebugUtils.h"
#include "GameObject.h"
#include "GameObjectState.h"

using namespace mk;

StateComponent::StateComponent(const std::string& stateId, std::unique_ptr<IState>&& startState)
{
	m_States.insert({ std::move(stateId), std::move(startState) });
}

void StateComponent::Start()
{
	IComponent::Start();
	// Add state has extra logic regarding ownership and needs to happen in start
	std::string key{ m_States.begin()->first };
	std::unique_ptr<IState> start{ std::move(m_States.begin()->second) };
	m_States.clear();
	AddState(std::move(key), std::move(start));
	SwitchState(m_States.begin()->first);
}

void StateComponent::FixedUpdate()
{
	IComponent::FixedUpdate();
	m_CurrentState->FixedUpdate();
}

void StateComponent::Update()
{
	IComponent::Update();
	m_CurrentState->Update();
}

void StateComponent::LateUpdate()
{
	IComponent::LateUpdate();
	m_CurrentState->LateUpdate();
	const std::string newState{ m_CurrentState->TransitionState() };
	if (m_States.contains(newState))
		SwitchState(newState);
	else if (!newState.empty())
		Print("State " + newState + "could not be switched to");
}

bool StateComponent::AddState(std::string stateId, std::unique_ptr<IState>&& state)
{
	if (m_States.contains(stateId))
	{
		Print("State already added with id " + stateId);
		return false;
	}

	if (GameObjectState* statePtr = dynamic_cast<GameObjectState*>(state.get()))
	{
		statePtr->SetOwner(GetOwner());
		statePtr->Start();
	}

	m_States.insert({ std::move(stateId), std::move(state) });
	return true;
}

void StateComponent::SwitchState(const std::string& stateId)
{
	if (m_CurrentState != nullptr)
		m_CurrentState->OnExit();
	m_CurrentState = m_States[stateId].get();
	m_CurrentState->OnEnter();
}
