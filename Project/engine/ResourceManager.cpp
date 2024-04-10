#include "ResourceManager.h"

using namespace mk;

Pipeline2D* ResourceManager::LoadShader2D(const std::string& shader)
{
	if (!m_Pipelines2D.contains(shader))
	{
		Pipeline2D pipeline{};
		pipeline.Initialize(shader);

		m_Pipelines2D[shader] = std::move(pipeline);
	}

	return &m_Pipelines2D[shader];
}

Pipeline3D* ResourceManager::LoadShader3D(const std::string& shader)
{
	if (!m_Pipelines3D.contains(shader))
	{
		Pipeline3D pipeline{};
		pipeline.Initialize(shader);

		m_Pipelines3D[shader] = std::move(pipeline);
	}

	return &m_Pipelines3D[shader];
}

void ResourceManager::Update()
{
	for (auto& pipeline : m_Pipelines2D)
		pipeline.second.Update();

	for (auto& pipeline : m_Pipelines3D)
		pipeline.second.Update();
}

void ResourceManager::Cleanup()
{
	for (auto& pipeline : m_Pipelines2D)
		pipeline.second.Destroy();

	for (auto& pipeline : m_Pipelines3D)
		pipeline.second.Destroy();
}
