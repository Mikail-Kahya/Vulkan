#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace mk;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Start()
{
	CleanupGameObjects();
}

void Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
		object->FixedUpdate();
}

void Scene::Update()
{
	for(auto& object : m_Objects)
		object->Update();
}

void Scene::LateUpdate()
{
	for (auto& object : m_Objects)
		object->LateUpdate();
	CleanupGameObjects();
}

GameObject* Scene::SpawnObject(const std::string& name)
{
	m_ObjectBuffer.emplace_back(std::make_unique<GameObject>(name));
	return m_ObjectBuffer.back().get();
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::CleanupGameObjects()
{
	// Remove destroy flagged components
	auto eraseIt = std::stable_partition(m_Objects.begin(), m_Objects.end(), [](const std::unique_ptr<GameObject>& object)
		{
			return !object->DestroyFlagged();
		});

	m_Objects.erase(eraseIt, m_Objects.end());

	// Move components and flush buffer
	m_Objects.insert(m_Objects.end(),
		std::make_move_iterator(m_ObjectBuffer.begin()),
		std::make_move_iterator(m_ObjectBuffer.end()));
	m_ObjectBuffer.clear();
}