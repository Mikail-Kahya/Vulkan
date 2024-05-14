#include "SceneManager.h"

using namespace mk;

void SceneManager::FixedUpdate()
{
	m_Scene->FixedUpdate();
}

void SceneManager::Update()
{
	m_Scene->Update();
}

void SceneManager::LateUpdate()
{
	m_Scene->LateUpdate();
}

TimeManager& SceneManager::GetTimeManager()
{
	return m_TimeManager;
}

const TimeManager& mk::Time()
{
	return SceneManager::GetInstance().GetTimeManager();
}

Scene& SceneManager::LoadScene(const std::string& name)
{
	m_Scene = std::make_unique<Scene>(name);
	return *m_Scene;
}

Scene& SceneManager::GetScene() const
{
	return *m_Scene;
}
