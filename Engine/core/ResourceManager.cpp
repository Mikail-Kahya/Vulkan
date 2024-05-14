#include "ResourceManager.h"

using namespace mk;

void ResourceManager::SetDefaultShaderPath(std::string path)
{
	s_ShaderPath = std::move(path);
}

void ResourceManager::SetDefaultTexturePath(std::string path)
{
	s_TexturePath = std::move(path);
}

Pipeline2D* ResourceManager::LoadShader2D(const std::string& shader)
{
	if (!m_Pipelines2D.contains(shader))
	{
		Pipeline2D pipeline{ };
		pipeline.Initialize(s_ShaderPath + shader);

		m_Pipelines2D[shader] = std::move(pipeline);
	}

	return &m_Pipelines2D[shader];
}

Pipeline3D* ResourceManager::LoadShader3D(const std::string& shader)
{
	if (!m_Pipelines3D.contains(shader))
	{
		Pipeline3D pipeline{ };
		pipeline.Initialize(s_ShaderPath + shader);

		m_Pipelines3D[shader] = std::move(pipeline);
	}

	return &m_Pipelines3D[shader];
}

Texture* ResourceManager::LoadTexture(const std::string& texture)
{
	if (!m_Textures.contains(texture))
		m_Textures[texture] = Texture{ s_TexturePath + texture };

	return &m_Textures[texture];
}

void ResourceManager::Cleanup()
{
	m_Pipelines2D.clear();
	m_Pipelines3D.clear();
}