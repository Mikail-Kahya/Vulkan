#include "Scene.h"

#include "Mesh2D.h"
#include "Pipeline2D.h"
#include "Pipeline3D.h"
#include "ResourceManager.h"

using namespace mk;

void Scene::Draw(uint32_t imageIdx) const
{
	std::vector<Mesh3D*> meshes3D{};
	for (const auto& meshSet : m_Mesh3DSets)
	{
		for (const auto& mesh : meshSet.second)
			meshes3D.push_back(mesh.get());

		meshSet.first->Draw(imageIdx, meshes3D);
		meshes3D.clear();
	}

	std::vector<Mesh2D*> meshes2D{};
	for (const auto& meshSet : m_Mesh2DSets)
	{
		for (const auto& mesh : meshSet.second)
			meshes2D.push_back(mesh.get());

		meshSet.first->Draw(imageIdx, meshes2D);
		meshes2D.clear();
	}
}

Mesh2D* Scene::AddMesh2D(const std::string& shader)
{
	Pipeline2D* pipeline = ResourceManager::GetInstance().LoadShader2D(shader);

	m_Mesh2DSets[pipeline].emplace_back(std::make_unique<Mesh2D>());

	return m_Mesh2DSets[pipeline].back().get();
}

Mesh3D* Scene::AddMesh3D(const std::string& shader)
{
	Pipeline3D* pipeline = ResourceManager::GetInstance().LoadShader3D(shader);

	m_Mesh3DSets[pipeline].emplace_back(std::make_unique<Mesh3D>());

	return m_Mesh3DSets[pipeline].back().get();
}

void Scene::RemoveMesh(Mesh2D* meshPtr)
{
	for (auto& meshSet : m_Mesh2DSets)
	{
		if (std::erase_if(meshSet.second, [meshPtr](const std::unique_ptr<Mesh2D>& mesh) { return mesh.get() == meshPtr; }))
			return;
	}
}

void Scene::RemoveMesh(Mesh3D* meshPtr)
{
	for (auto& meshSet : m_Mesh3DSets)
	{
		if (std::erase_if(meshSet.second, [meshPtr](const std::unique_ptr<Mesh3D>& mesh) { return mesh.get() == meshPtr; }))
			return;
	}
}
