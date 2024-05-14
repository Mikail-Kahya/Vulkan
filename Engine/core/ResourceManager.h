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

		static Pipeline2D* LoadShader2D(const std::string& vertex, const std::string& fragment);
		static Pipeline3D* LoadShader3D(const std::string& vertex, const std::string& fragment);
		static Texture* LoadTexture(const std::string& texture);
		static void Cleanup();

	private:
		

		inline static std::string s_ShaderPath{};
		inline static std::string s_TexturePath{};

		inline static std::map<std::string, Pipeline2D> m_Pipelines2D{};
		inline static std::map<std::string, Pipeline3D> m_Pipelines3D{};
		inline static std::map<std::string, Texture> m_Textures{};
	};
}
