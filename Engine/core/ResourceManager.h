#pragma once
#include <map>
#include <string>

#include "abstraction/Pipeline2D.h"
#include "abstraction/Pipeline3D.h"
#include "abstraction/Texture.h"

namespace mk
{
	class ResourceManager final
	{
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		ResourceManager(const ResourceManager& other)					= delete;
		ResourceManager(ResourceManager&& other) noexcept				= delete;
		ResourceManager& operator=(const ResourceManager& other)		= delete;
		ResourceManager& operator=(ResourceManager&& other) noexcept	= delete;

		static void SetDefaultShaderPath(std::string path);
		static void SetDefaultTexturePath(std::string path);
		static void SetDefaultMeshPath(std::string path);

		static Pipeline2D* LoadShader2D(const std::string& vertex, const std::string& fragment);
		static Pipeline3D* LoadShader3D(const std::string& vertex, const std::string& fragment);
		static Texture* LoadTexture(const std::string& texture);
		static Mesh3D* LoadMesh3D(const std::string& obj);
		static Mesh2D* LoadMesh2D(const std::string& obj);
		static void Cleanup();

	private:
		

		inline static std::string s_ShaderPath{};
		inline static std::string s_TexturePath{};
		inline static std::string s_MeshPath{};

		inline static std::map<std::string, Pipeline2D> s_Pipelines2D{};
		inline static std::map<std::string, Pipeline3D> s_Pipelines3D{};
		inline static std::map<std::string, Texture> s_Textures{};
		inline static std::map<std::string, std::unique_ptr<Mesh3D>> s_Meshes3D{};
		inline static std::map<std::string, std::unique_ptr<Mesh2D>> s_Meshes2D{};
	};
}
