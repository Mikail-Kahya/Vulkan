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

void ResourceManager::SetDefaultMeshPath(std::string path)
{
	s_MeshPath = std::move(path);
}

Pipeline2D* ResourceManager::LoadShader2D(const std::string& vertex, const std::string& fragment)
{
	const std::string key{ vertex + fragment };
	if (!s_Pipelines2D.contains(key))
	{
		Pipeline2D pipeline{ };
		pipeline.Initialize(s_ShaderPath + vertex, s_ShaderPath + fragment);

		s_Pipelines2D[key] = std::move(pipeline);
	}

	return &s_Pipelines2D[key];
}

Pipeline3D* ResourceManager::LoadShader3D(const std::string& vertex, const std::string& fragment)
{
	const std::string key{ vertex + fragment };
	if (!s_Pipelines3D.contains(key))
	{
		Pipeline3D pipeline{ };
		pipeline.Initialize(s_ShaderPath + vertex, s_ShaderPath + fragment);

		s_Pipelines3D[key] = std::move(pipeline);
	}

	return &s_Pipelines3D[key];
}

Texture* ResourceManager::LoadTexture(const std::string& texture)
{
	if (!s_Textures.contains(texture))
		s_Textures[texture] = Texture{ s_TexturePath + texture };
	return &s_Textures[texture];
}

Mesh3D* ResourceManager::LoadMesh3D(const std::string& obj, bool invert)
{
	if (!s_Meshes3D.contains(obj))
	{
		s_Meshes3D[obj] = std::make_unique<Mesh3D>();
		s_Meshes3D[obj]->Load(s_MeshPath + obj, invert);
	}
	return s_Meshes3D[obj].get();
}

Mesh2D* ResourceManager::LoadMesh2D(const std::string& obj)
{
	if (!s_Meshes2D.contains(obj))
	{
		s_Meshes2D[obj] = std::make_unique<Mesh2D>();
		s_Meshes2D[obj]->Load(s_MeshPath + obj);
	}
	return s_Meshes2D[obj].get();
}

void ResourceManager::Cleanup()
{
	s_Pipelines2D.clear();
	s_Pipelines3D.clear();
	s_Meshes2D.clear();
	s_Meshes3D.clear();
	s_Textures.clear();
}
