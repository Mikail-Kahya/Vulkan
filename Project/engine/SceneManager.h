#pragma once
#include <map>
#include <string>

#include "Scene.h"
#include "interfaces/Singleton.h"

namespace mk
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager()				= default;
		~SceneManager() override	= default;

		SceneManager(const SceneManager& other)					= delete;
		SceneManager(SceneManager&& other) noexcept				= delete;
		SceneManager& operator=(const SceneManager& other)		= delete;
		SceneManager& operator=(SceneManager&& other) noexcept	= delete;

		void Draw() const;
		void Cleanup();

		Scene* LoadScene(const std::string& sceneName);
		void RemoveScene(const std::string& sceneName);

	private:
		Scene* m_CurrentScenePtr{};
		std::map<std::string, Scene> m_Scenes{};
	};
}