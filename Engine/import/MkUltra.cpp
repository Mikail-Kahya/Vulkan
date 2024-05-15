#include <stdexcept>
#include <sstream>
#include <iostream>
#include <thread>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


#include "MkUltra.h"
#include "InputManager.h"
#include "PhysicsSystem.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

void LogSDLVersion(const std::string& message, const SDL_version& v)
{
#if WIN32
	std::stringstream ss;
	ss << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<mk::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	LogSDLVersion("We compiled against SDL version ", version);

	SDL_GetVersion(&version);
	LogSDLVersion("We linked against SDL version ", version);

	SDL_IMAGE_VERSION(&version);
	LogSDLVersion("We compiled against SDL_image version ", version);

	version = *IMG_Linked_Version();
	LogSDLVersion("We linked against SDL_image version ", version);

	SDL_TTF_VERSION(&version)
	LogSDLVersion("We compiled against SDL_ttf version ", version);

	version = *TTF_Linked_Version();
	LogSDLVersion("We linked against SDL_ttf version ", version);
}

mk::MkUltra::MkUltra(const std::filesystem::path &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	Renderer::GetInstance().Init(720, 480);
	ResourceManager::GetInstance().Init(dataPath);
}

mk::MkUltra::~MkUltra()
{
	Renderer::GetInstance().Destroy();
	SDL_Quit();
}

void mk::MkUltra::Run(const std::function<void()>& load)
{
	load();

	// Setup of timing
	using namespace std::chrono;
	m_LastTime = high_resolution_clock::now();
	SceneManager::GetInstance().GetTimeManager().fixedDeltaTime = FIXED_TIME_STEP;

#ifndef __EMSCRIPTEN__
	while (!m_quit)
		RunOneFrame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void mk::MkUltra::RunOneFrame()
{
	using namespace std::chrono;
	SceneManager& sceneManager{ SceneManager::GetInstance() };
	Renderer& renderer{ Renderer::GetInstance() };

	m_quit = !InputManager::GetInstance().ProcessInput();

	const auto currentTime{ high_resolution_clock::now() };
	const float deltaTime{ duration<float>(currentTime - m_LastTime).count()};
	m_LastTime = currentTime;
	m_Lag += deltaTime;

	// Update global time
	TimeManager& timeManager{ sceneManager.GetTimeManager() };
	timeManager.deltaTime = deltaTime;

	while (m_Lag >= FIXED_TIME_STEP)
	{
		SceneManager::GetInstance().FixedUpdate();
		PhysicsSystem::GetInstance().Update();
		m_Lag -= FIXED_TIME_STEP;
	}

	sceneManager.Update();
	sceneManager.LateUpdate();

	renderer.Update();
	renderer.Render();

	constexpr milliseconds msPerFrame{ static_cast<long long>(1.f / FPS * 1000.f) };
	const auto sleepTime{ currentTime + msPerFrame - high_resolution_clock::now()};

	std::this_thread::sleep_for(sleepTime);
}
