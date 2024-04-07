#include "Scene.h"

#include "Mesh.h"
#include "Pipeline.h"
#include "ResourceManager.h"

using namespace mk;

void Scene::Draw(uint32_t imageIdx) const
{
	for (const auto& mesh : m_Meshes)
		mesh.first->Draw(imageIdx);
}

Mesh* Scene::AddMesh(const std::string& shader)
{
	Pipeline* pipeline = ResourceManager::GetInstance().LoadShader(shader);
	
	std::unique_ptr<Mesh> mesh{ std::make_unique<Mesh>() };
	Mesh* meshPtr{ mesh.get() };
	m_Meshes.emplace_back(std::make_pair(pipeline, std::move(mesh)));
	return meshPtr;
}

void Scene::RemoveMesh(Mesh* meshPtr)
{
	const auto foundIter = std::find_if(m_Meshes.begin(), m_Meshes.end(), [meshPtr](const AllocatedMesh& mesh)
		{
			return mesh.second.get() == meshPtr;
		});

	m_Meshes.erase(foundIter);
}
