#pragma once
#include <map>
#include <string>

#include "interfaces/Singleton.h"
#include "abstraction/Pipeline2D.h"
#include "abstraction/Pipeline3D.h"
#include "abstraction/Texture.h"

namespace mk
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
		friend class Singleton<ResourceManager>;
	public:
		
		~ResourceManager() override	= default;

		ResourceManager(const ResourceManager& other)					= delete;
		ResourceManager(ResourceManager&& other) noexcept				= delete;
		ResourceManager& operator=(const ResourceManager& other)		= delete;
		ResourceManager& operator=(ResourceManager&& other) noexcept	= delete;

		static void SetDefaultShaderPath(std::string path);
		static void SetDefaultTexturePath(std::string path);

		Pipeline2D* LoadShader2D(const std::string& shader);
		Pipeline3D* LoadShader3D(const std::string& shader);
		Texture* LoadTexture(const std::string& texture);
		void Cleanup();

	private:
		ResourceManager() = default;

		inline static std::string s_ShaderPath{};
		inline static std::string s_TexturePath{};

		std::map<std::string, Pipeline2D> m_Pipelines2D{};
		std::map<std::string, Pipeline3D> m_Pipelines3D{};
		std::map<std::string, Texture> m_Textures{};
	};
}
