#pragma once
#include "GameObject.h"

namespace mk
{
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other)				= delete;
		Scene(Scene&& other)					= delete;
		Scene& operator=(const Scene& other)	= delete;
		Scene& operator=(Scene&& other)			= delete;

		void Start();
		
		void FixedUpdate();
		void Update();
		void LateUpdate();

		GameObject* SpawnObject(const std::string& name = { "None" });
		void RemoveAll();

	private: 
		void CleanupGameObjects();

		std::string m_Name;
		std::vector <std::unique_ptr<GameObject>> m_Objects{};
		std::vector <std::unique_ptr<GameObject>> m_ObjectBuffer{};

		static unsigned int m_IdCounter; 
	};

}
