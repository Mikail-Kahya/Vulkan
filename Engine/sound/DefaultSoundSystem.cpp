#include "DefaultSoundSystem.h"

#include <chrono>
#include <unordered_map>

#include "soloud.h"
#include "soloud_wav.h"

using namespace mk;

class CachedSound final
{
public:
	CachedSound() = default;
	CachedSound(sound_id id) : m_WaveFile{ std::make_unique<SoLoud::Wav>() }
	{
		m_WaveFile->load(id.c_str());
		constexpr float defaultCacheTime{ 10.f };
		m_CacheTime = defaultCacheTime + static_cast<float>(m_WaveFile->getLength());
		SetTime();
	}
	~CachedSound() = default;

	CachedSound(const CachedSound& other) = delete;
	CachedSound(CachedSound&& other) noexcept = default;
	CachedSound& operator=(const CachedSound& other) = delete;
	CachedSound& operator=(CachedSound&& other)	noexcept = default;

	SoLoud::Wav& GetSound()
	{
		SetTime();
		return *m_WaveFile;
	}
	bool ShouldDestroy() const
	{
		using namespace std::chrono;
		const steady_clock::time_point currentTime{ steady_clock::now() };
		const float deltaTime{ duration<float>(currentTime - m_LastUsedTime).count() };
		return m_CacheTime < deltaTime;
	}

private:
	void SetTime() { m_LastUsedTime = std::chrono::steady_clock::now(); }

	std::chrono::steady_clock::time_point m_LastUsedTime;
	std::unique_ptr<SoLoud::Wav> m_WaveFile{};
	float m_CacheTime{};

};

struct Handle
{
	CachedSound* CachedSoundPtr{};
	SoLoud::handle SoloudHandle{};
	bool IsUsed{};
};

class DefaultSoundSystem::SoloudImpl final
{
public:
	SoloudImpl() { m_Souloud.init(); }
	~SoloudImpl() { m_Souloud.deinit(); }

	void SetDefaultDataPath(const std::string& dataPath) { m_DefaultDataPath = dataPath; }
	void Update();

	void Play(sound_id id, float volume, handle_id handleId);
	void SetPause(handle_id soundHandleId, bool pause) { m_Souloud.setPause(m_Handles[soundHandleId].SoloudHandle, pause); }
	void Stop(handle_id soundHandleId);
	void StopAll() { m_Souloud.stopAll(); }

	handle_id GetNextHandleId();
	bool HandleOutOfRange(handle_id soundHandle) const;

private:
	void Load(sound_id id) { m_CachedSounds[id] = CachedSound{ m_DefaultDataPath + id }; }
	void SetHandleNull(handle_id handleId);

	SoLoud::Soloud m_Souloud{};
	std::unordered_map<sound_id, CachedSound> m_CachedSounds{};
	std::vector<Handle> m_Handles{};
	std::string m_DefaultDataPath{};
};

void DefaultSoundSystem::SoloudImpl::Update()
{
	for (Handle& handle : m_Handles)
		handle.IsUsed = m_Souloud.isValidVoiceHandle(handle.SoloudHandle);

	std::erase_if(m_CachedSounds, [](const auto& cachedSound)
		{
			return cachedSound.second.ShouldDestroy();
		});
}

void DefaultSoundSystem::SoloudImpl::Play(sound_id id, float volume, handle_id handleId)
{
	if (!m_CachedSounds.contains(id))
		Load(id);

 	const Handle handle{ &m_CachedSounds[id], m_Souloud.play(m_CachedSounds[id].GetSound(), volume), true };
	if (handleId == m_Handles.size())
		m_Handles.emplace_back(handle);
	else
		m_Handles[handleId] = handle;
}

void DefaultSoundSystem::SoloudImpl::Stop(handle_id soundHandleId)
{
	m_Souloud.stop(m_Handles[soundHandleId].SoloudHandle);
	SetHandleNull(soundHandleId);
}

handle_id DefaultSoundSystem::SoloudImpl::GetNextHandleId()
{
	const auto foundId = std::find_if(m_Handles.begin(), m_Handles.end(), [](const Handle& handle)
	{
			return handle.IsUsed;
	});

	return foundId == m_Handles.end() ? m_Handles.size() : std::distance(m_Handles.begin(), foundId);
}

bool DefaultSoundSystem::SoloudImpl::HandleOutOfRange(handle_id soundHandleId) const
{
	return m_Handles.size() - 1 < soundHandleId;
}

void DefaultSoundSystem::SoloudImpl::SetHandleNull(handle_id handleId)
{
	m_Handles[handleId].IsUsed = false;
}

// ----------- System -----------

DefaultSoundSystem::DefaultSoundSystem()
	: ISoundSystem()
	, m_Impl{ std::make_unique<SoloudImpl>() }
	, m_Thread()
{
	SetupThread();
}

DefaultSoundSystem::~DefaultSoundSystem()
{
	m_CloseThread = true;
	m_QueueState.notify_all();
}

void DefaultSoundSystem::SetupThread()
{
	m_Thread = std::jthread([this]()
	{
		while (!m_CloseThread)
		{
			// wait
			std::unique_lock<std::mutex> transferLock{ m_QueueMutex };
			m_QueueState.wait(transferLock, [this] { return !m_Events.empty() || m_CloseThread; });

			// move everything to local queue
			std::queue<Event> events{};
			events.swap(m_Events);

			transferLock.unlock();

			// perform functions of local queue
			while (!events.empty())
			{
				events.back()(m_Impl);
				events.pop();
			}

			// Remove sounds if not used after events are handled
			transferLock.lock();
			m_Impl->Update();
		}
	});
}

void DefaultSoundSystem::Lock()
{
	m_QueueMutex.lock();
}

void DefaultSoundSystem::Unlock()
{
	m_QueueMutex.unlock();
	m_QueueState.notify_all();
}


void DefaultSoundSystem::SetDefaultDataPath(const std::string& dataPath)
{
	m_Impl->SetDefaultDataPath(dataPath);
}

handle_id DefaultSoundSystem::Play(const sound_id& id, float volume)
{
	std::lock_guard lock{ m_QueueMutex };
	const handle_id handleId{ m_Impl->GetNextHandleId() };
	m_Events.emplace([id, volume, handleId](const Impl& impl) mutable { impl->Play(std::move(id), volume, handleId); });
	m_QueueState.notify_all();
	return handleId;
}

void DefaultSoundSystem::Pause(handle_id soundHandle)
{
	std::lock_guard lock{ m_QueueMutex };
	if (m_Impl->HandleOutOfRange(soundHandle))
		return;

	m_Events.emplace([soundHandle](const Impl& impl) { impl->SetPause(soundHandle, true); });
	m_QueueState.notify_all();
}

void DefaultSoundSystem::Unpause(handle_id soundHandleId)
{
	std::lock_guard lock{ m_QueueMutex };
	if (m_Impl->HandleOutOfRange(soundHandleId))
		return;

	m_Events.emplace([soundHandleId](const Impl& impl) { impl->SetPause(soundHandleId, false); });
	m_QueueState.notify_all();
}

void DefaultSoundSystem::Stop(handle_id soundHandleId)
{
	std::lock_guard lock{ m_QueueMutex };
	if (m_Impl->HandleOutOfRange(soundHandleId))
		return;

	m_Events.emplace([soundHandleId](const Impl& impl) { impl->Stop(soundHandleId); });
	m_QueueState.notify_all();
}

void DefaultSoundSystem::StopAll()
{
	std::lock_guard lock{ m_QueueMutex };
	m_Events.emplace([](const Impl& impl) { impl->StopAll(); });
	m_QueueState.notify_all();
}