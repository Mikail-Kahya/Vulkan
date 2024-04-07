#include "ResourceManager.h"

#include "Pipeline.h"

using namespace mk;

Pipeline* ResourceManager::LoadShader(const std::string& shader)
{
	if (!m_Pipelines.contains(shader))
	{
		Pipeline pipeline{};
		pipeline.Initialize(shader);

		m_Pipelines[shader] = std::move(pipeline);
	}

	return &m_Pipelines[shader];
}

void ResourceManager::Update()
{
	for (auto& pipeline : m_Pipelines)
		pipeline.second.Update();
}

void ResourceManager::Cleanup()
{
	for (auto& pipeline : m_Pipelines)
		pipeline.second.Destroy();
}
