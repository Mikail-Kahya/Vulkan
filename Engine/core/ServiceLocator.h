#pragma once
#include "ISoundSystem.h"

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

		template <std::derived_from<ISoundSystem> SystemType, typename ...Args>
		static void RegisterSoundSystem(const Args&... args)
		{
			s_SoundSystem = std::make_unique<SystemType>(args...);
			s_SoundSystem->SetDefaultDataPath(m_DataPath);
		}
		static ISoundSystem& GetSoundSystem();

		static void SetDefaultDataPath(const std::string& dataPath);

	private:
		ServiceLocator() = default;

		// services
		static std::unique_ptr<ISoundSystem> s_SoundSystem;
		inline static std::string m_DataPath{};
	};
}
