#include <stdexcept>
#include <thread>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif


#include "MkVulktra.h"
#include "Core/ServiceLocator.h"
#include "Core/SceneManager.h"
#include "Core/ResourceManager.h"
#include "core/Time.h"
#include "input/GLFWInput.h"
#include "render/VulkanRenderer.h"
#include "sound/DefaultSoundSystem.h"

mk::MkVulktra::MkVulktra()
{
	VulkanBase::GetInstance().InitVulkan();
}

mk::MkVulktra::~MkVulktra()
{
	ResourceManager::Cleanup();
	VulkanBase::GetInstance().Cleanup();
}

void mk::MkVulktra::Run(const std::function<void()>& load)
{
	ServiceLocator::RegisterRenderer(std::make_unique<VulkanRenderer>(1000, 500));
	//ServiceLocator::RegisterSoundSystem(std::make_unique<DefaultSoundSystem>());
	ServiceLocator::RegisterInputSystem(std::make_unique<GLFWInput>(VulkanBase::GetInstance().GetWindow()));
	load();
	Time::Update();

#ifndef __EMSCRIPTEN__
	while (!m_quit)
		RunOneFrame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void mk::MkVulktra::RunOneFrame()
{
	using namespace std::chrono;
	SceneManager& sceneManager{ SceneManager::GetInstance() };

	m_quit = !ServiceLocator::GetInputSystem().ProcessInput();

	const auto currentTime{ high_resolution_clock::now() };
	m_Lag += Time::DeltaTime();

	// Update global time
	Time::Update();

	while (m_Lag >= FIXED_TIME_STEP)
	{
		SceneManager::GetInstance().FixedUpdate();
		m_Lag -= FIXED_TIME_STEP;
	}

	sceneManager.Update();
	sceneManager.LateUpdate();

	ServiceLocator::GetRenderer().Update();
	ServiceLocator::GetRenderer().Render();

	constexpr milliseconds msPerFrame{ static_cast<long long>(1.f / FPS * 1000.f) };
	const auto sleepTime{ currentTime + msPerFrame - high_resolution_clock::now()};

	std::this_thread::sleep_for(sleepTime);
}
