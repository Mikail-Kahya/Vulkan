#pragma once

#include "ISoundSystem.h"

namespace mk
{
	class NullSoundSystem : public ISoundSystem
	{
	public:
		NullSoundSystem()			= default;
		~NullSoundSystem() override = default;

		NullSoundSystem(const NullSoundSystem& other)					= delete;
		NullSoundSystem(NullSoundSystem&& other) noexcept				= delete;
		NullSoundSystem& operator=(const NullSoundSystem& other)		= delete;
		NullSoundSystem& operator=(NullSoundSystem&& other)	noexcept	= delete;

		void SetDefaultDataPath(const std::string&) override{}
		handle_id Play(const sound_id&, float) override { return 0; }
		void Pause(handle_id) override{}
		void Unpause(handle_id) override{}
		void Stop(handle_id) override{}
		void StopAll() override{}
	};
}