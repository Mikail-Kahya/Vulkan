#include "SceneManager.h"

using namespace mk;

void SceneManager::FixedUpdate()
{
	if (m_Scene)
		m_Scene->FixedUpdate();
}

void SceneManager::Update()
{
	if (m_Scene)
		m_Scene->Update();
}

void SceneManager::LateUpdate()
{
	if (m_Scene)
		m_Scene->LateUpdate();
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
