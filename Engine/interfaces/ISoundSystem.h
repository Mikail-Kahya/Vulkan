#pragma once
#include <future>
#include <string>

namespace mk
{
	using sound_id = std::string;
	using handle_id = size_t;

	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void SetDefaultDataPath(const std::string& dataPath) = 0;

		virtual handle_id Play(const sound_id& id, float volume) = 0;
		virtual void Pause(handle_id soundHandle) = 0;
		virtual void Unpause(handle_id soundHandle) = 0;
		virtual void Stop(handle_id soundHandle) = 0;
		virtual void StopAll() = 0;

	protected:
		ISoundSystem() = default;
		ISoundSystem(const ISoundSystem& other)					= delete;
		ISoundSystem(ISoundSystem&& other) noexcept				= delete;
		ISoundSystem& operator=(const ISoundSystem& other)		= delete;
		ISoundSystem& operator=(ISoundSystem&& other)	noexcept	= delete;
	};
}
