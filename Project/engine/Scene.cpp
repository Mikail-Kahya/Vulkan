#include "Scene.h"

#include "Mesh.h"
#include "Pipeline.h"
#include "ResourceManager.h"

using namespace mk;

void Scene::Draw(uint32_t imageIdx) const
{
	std::vector<Mesh*> meshes{};
	for (const auto& meshSet : m_MeshSets)
	{
		for (const auto& mesh : meshSet.second)
			meshes.push_back(mesh.get());

		meshSet.first->Draw(imageIdx, meshes);
		meshes.clear();
	}
		
}

Mesh* Scene::AddMesh(const std::string& shader)
{
	Pipeline* pipeline = ResourceManager::GetInstance().LoadShader(shader);

	m_MeshSets[pipeline].emplace_back(std::make_unique<Mesh>());

	return m_MeshSets[pipeline].back().get();
}

void Scene::RemoveMesh(Mesh* meshPtr)
{
	for (auto& meshSet : m_MeshSets)
	{
		if (std::erase_if(meshSet.second, [meshPtr](const std::unique_ptr<Mesh>& mesh) { return mesh.get() == meshPtr; }))
			return;
	}
}
