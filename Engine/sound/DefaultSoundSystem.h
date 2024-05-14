#pragma once
#include <functional>
#include <memory>
#include <queue>

#include "ISoundSystem.h"

namespace mk
{
	class DefaultSoundSystem final : public ISoundSystem
	{
	public:
		DefaultSoundSystem();
		~DefaultSoundSystem() override;

		DefaultSoundSystem(const DefaultSoundSystem& other)					= delete;
		DefaultSoundSystem(DefaultSoundSystem&& other) noexcept				= delete;
		DefaultSoundSystem& operator=(const DefaultSoundSystem& other)		= delete;
		DefaultSoundSystem& operator=(DefaultSoundSystem&& other) noexcept	= delete;

		void SetDefaultDataPath(const std::string& dataPath) override;
		handle_id Play(const sound_id& id, float volume) override;
		void Pause(handle_id soundHandleId) override;
		void Unpause(handle_id soundHandleId) override;
		void Stop(handle_id soundHandleId) override;
		void StopAll() override;

	private:
		void SetupThread();
		void Lock();
		void Unlock();

		class SoloudImpl;
		using Impl = std::unique_ptr<SoloudImpl>;
		using Event = std::function<void(const Impl&)>;

		std::queue<Event> m_Events{};
		Impl m_Impl;
		std::condition_variable m_QueueState{};
		std::jthread m_Thread;
		std::mutex m_QueueMutex{};
		bool m_CloseThread{ false };
	};
}