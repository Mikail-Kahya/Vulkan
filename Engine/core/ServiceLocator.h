#pragma once
#include "sound/ISoundSystem.h"
#include "render/IRenderer.h"

namespace mk
{
	class ServiceLocator final
	{
	public:
		~ServiceLocator() = default;

		ServiceLocator(const ServiceLocator& other)					= delete;
		ServiceLocator(ServiceLocator&& other) noexcept				= delete;
		ServiceLocator& operator=(const ServiceLocator& other)		= delete;
		ServiceLocator& operator=(ServiceLocator&& other) noexcept	= delete;


		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& soundSystem);
		static void RegisterSoundSystem(std::unique_ptr<IRenderer>&& renderer);

		static ISoundSystem& GetSoundSystem();
		static IRenderer& GetRenderer();

		static void SetDefaultDataPath(const std::string& dataPath);

	private:
		ServiceLocator() = default;

		// services
		static std::unique_ptr<ISoundSystem> s_SoundSystem;
		static std::unique_ptr<IRenderer> s_Renderer;
		inline static std::string m_DataPath{};
	};
}
