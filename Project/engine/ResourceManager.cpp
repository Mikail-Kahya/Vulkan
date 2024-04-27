#include "ResourceManager.h"

using namespace mk;

Pipeline2D* ResourceManager::LoadShader2D(const std::string& shader, bool canClear)
{
	if (!m_Pipelines2D.contains(shader))
	{
		Pipeline2D pipeline{ };
		pipeline.Initialize(shader);

		m_Pipelines2D[shader] = std::move(pipeline);
	}

	return &m_Pipelines2D[shader];
}

Pipeline3D* ResourceManager::LoadShader3D(const std::string& shader, bool canClear)
{
	if (!m_Pipelines3D.contains(shader))
	{
		Pipeline3D pipeline{ };
		pipeline.Initialize(shader);

		m_Pipelines3D[shader] = std::move(pipeline);
	}

	return &m_Pipelines3D[shader];
}

void ResourceManager::Update()
{
}

void ResourceManager::Cleanup()
{
	m_Pipelines2D.clear();
	m_Pipelines3D.clear();
}
