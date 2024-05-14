#include "ServiceLocator.h"
#include "sound/NullSoundSystem.h"
#include "render/NullRenderer.h"
using namespace mk;

std::unique_ptr<ISoundSystem> ServiceLocator::s_SoundSystem{ std::make_unique<NullSoundSystem>() };
std::unique_ptr<IRenderer> ServiceLocator::s_Renderer{ std::make_unique<NullRenderer>() };


void ServiceLocator::RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& soundSystem)
{
	s_SoundSystem = (soundSystem) ? std::move(soundSystem) : std::make_unique<NullSoundSystem>();
	s_SoundSystem->SetDefaultDataPath(m_DataPath);
}

void ServiceLocator::RegisterSoundSystem(std::unique_ptr<IRenderer>&& renderer)
{
	s_Renderer = (renderer) ? std::move(renderer) : std::make_unique<NullRenderer>();
}

ISoundSystem& ServiceLocator::GetSoundSystem()
{
	return *s_SoundSystem;
}

IRenderer& ServiceLocator::GetRenderer()
{
	return *s_Renderer;
}

void ServiceLocator::SetDefaultDataPath(const std::string& dataPath)
{
	GetSoundSystem().SetDefaultDataPath(dataPath);
	m_DataPath = dataPath;
}
