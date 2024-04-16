#pragma once
#include <map>
#include <string>

#include "interfaces/Singleton.h"
#include "Pipeline2D.h"
#include "Pipeline3D.h"

namespace mk
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager()			= default;
		~ResourceManager() override	= default;

		ResourceManager(const ResourceManager& other)					= delete;
		ResourceManager(ResourceManager&& other) noexcept				= delete;
		ResourceManager& operator=(const ResourceManager& other)		= delete;
		ResourceManager& operator=(ResourceManager&& other) noexcept	= delete;

		Pipeline2D* LoadShader2D(const std::string& shader, bool canClear = false);
		Pipeline3D* LoadShader3D(const std::string& shader, bool canClear = false);
		void Update();
		void Cleanup();

	private:
		std::map<std::string, Pipeline2D> m_Pipelines2D{};
		std::map<std::string, Pipeline3D> m_Pipelines3D{};
	};
}
