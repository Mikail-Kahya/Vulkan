#include "Time.h"

using namespace mk;

void Time::Update()
{
	using namespace std::chrono;
	high_resolution_clock::time_point now{ high_resolution_clock::now() };
	m_DeltaTime = duration<float>(now - m_PrevTime).count();
	m_PrevTime = now;
}

float Time::DeltaTime()
{
	return m_DeltaTime;
}
