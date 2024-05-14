#pragma once
#include <string>
#include <memory>
#include "ISingleton.h"
#include "TimeManager.h"
#include "Scene.h"

namespace mk
{
	// Design for future when loading from files
		// Load scene with file, allow preloading, and hold one current scene 

	class SceneManager final : public ISingleton<SceneManager>
	{
		friend class ISingleton<SceneManager>;
	public:
		
		~SceneManager() override = default;

		SceneManager(const SceneManager& other)					= delete;
		SceneManager(SceneManager&& other) noexcept				= delete;
		SceneManager& operator=(const SceneManager& other)		= delete;
		SceneManager& operator=(SceneManager&& other) noexcept	= delete;

		Scene& LoadScene(const std::string& name);
		Scene& GetScene() const;

		void FixedUpdate();
		void Update();
		void LateUpdate();

		TimeManager& GetTimeManager();

	private:
		SceneManager() = default;

		std::unique_ptr<Scene> m_Scene{};
		TimeManager m_TimeManager{};
	};

	const TimeManager& Time();
}
