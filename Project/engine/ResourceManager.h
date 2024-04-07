#pragma once
#include <map>
#include <string>

#include "interfaces/Singleton.h"

namespace mk
{
	class Mesh;
	class Pipeline;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager()			= default;
		~ResourceManager() override	= default;

		ResourceManager(const ResourceManager& other)					= delete;
		ResourceManager(ResourceManager&& other) noexcept				= delete;
		ResourceManager& operator=(const ResourceManager& other)		= delete;
		ResourceManager& operator=(ResourceManager&& other) noexcept	= delete;

		Pipeline* LoadShader(const std::string& shader);
		void Update();
		void Cleanup();

	private:
		std::map<std::string, Pipeline> m_Pipelines{};
	};
}