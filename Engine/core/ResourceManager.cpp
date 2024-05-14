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

Pipeline2D* ResourceManager::LoadShader2D(const std::string& vertex, const std::string& fragment)
{
	const std::string key{ vertex + fragment };
	if (!m_Pipelines2D.contains(key))
	{
		Pipeline2D pipeline{ };
		pipeline.Initialize(s_ShaderPath + vertex, s_ShaderPath + fragment);

		m_Pipelines2D[key] = std::move(pipeline);
	}

	return &m_Pipelines2D[key];
}

Pipeline3D* ResourceManager::LoadShader3D(const std::string& vertex, const std::string& fragment)
{
	const std::string key{ vertex + fragment };
	if (!m_Pipelines3D.contains(key))
	{
		Pipeline3D pipeline{ };
		pipeline.Initialize(s_ShaderPath + vertex, s_ShaderPath + fragment);

		m_Pipelines3D[key] = std::move(pipeline);
	}

	return &m_Pipelines3D[key];
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