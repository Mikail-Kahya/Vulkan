#include "Time.h"

using namespace mk;
using namespace std::chrono;

high_resolution_clock::time_point Time::m_Start { high_resolution_clock::now() };
high_resolution_clock::time_point Time::m_LastUpdated{ high_resolution_clock::now() };

void Time::Update()
{
	high_resolution_clock::time_point now{ high_resolution_clock::now() };
	m_DeltaTime = duration<float>(now - m_LastUpdated).count();
	m_LastUpdated = now;
}

float Time::TotalElapsedTime()
{
	using namespace std::chrono;
	return duration<float>(high_resolution_clock::now() - m_Start).count();
}

float Time::FPS()
{
	return 1 / m_DeltaTime;
}

float Time::DeltaTime()
{
	return m_DeltaTime;
}
