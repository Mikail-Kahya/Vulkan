#include "SceneManager.h"

using namespace mk;

void SceneManager::Draw() const
{
	m_CurrentScenePtr->Draw();
}

void SceneManager::Cleanup()
{
	m_CurrentScenePtr = nullptr;
	m_Scenes.clear();
}

Scene* SceneManager::LoadScene(const std::string& sceneName)
{
	m_CurrentScenePtr = &m_Scenes[sceneName];
	
	return m_CurrentScenePtr;
}

void SceneManager::RemoveScene(const std::string& sceneName)
{
	if (!m_Scenes.contains(sceneName))
		return;

	m_Scenes.erase(sceneName);
}
