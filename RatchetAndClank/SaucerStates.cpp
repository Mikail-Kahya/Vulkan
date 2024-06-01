#include "SaucerStates.h"

#include "core/GameObject.h"
#include "core/Time.h"
#include "glm/ext/quaternion_geometric.hpp"

using namespace mk;

void LandingState::Start()
{
	GameObjectState::Start();
	m_LandingSpot = GetOwner()->GetLocalPosition();
}

void LandingState::Update()
{
	GameObjectState::Update();

	m_Timer += Time::DeltaTime();
	const float alpha{ m_Timer / LANDING_TIME };

	GetOwner()->SetLocalPosition((1 - alpha) * m_StartingSpot + alpha * m_LandingSpot);
}

void LandingState::OnEnter()
{
	GameObjectState::OnEnter();
	m_Timer = 0;
	m_StartingSpot = GetOwner()->GetLocalPosition();
}

std::string LandingState::TransitionState()
{
	if (m_Timer > LANDING_TIME)
		return "Hover";
	return "";
}

void FlyingState::OnEnter()
{
	GameObjectState::OnEnter();
	m_Timer = 0;
	constexpr int randPrecision{ 10000 };
	m_FlyingDirection = glm::normalize(glm::vec3{
		rand() % randPrecision - randPrecision * 0.5f,
		rand() % randPrecision / 10,
		rand() % randPrecision - randPrecision * 0.5f
	});
}

void FlyingState::Update()
{
	GameObjectState::Update();
	m_Timer += Time::DeltaTime();

	GetOwner()->AddYaw(ROTATION_SPEED * Time::DeltaTime());
	GetOwner()->AddLocalOffset(FLYING_SPEED * Time::DeltaTime() * m_FlyingDirection);
}

std::string FlyingState::TransitionState()
{
	if (m_Timer > FLYING_TIME)
		return "Land";
	return "";
}
