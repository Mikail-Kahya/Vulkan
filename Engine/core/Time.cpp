#include "Time.h"

using namespace mk;

Time::Time()
	: start{ std::chrono::steady_clock::now() }
{
}

float Time::GetTotalTime() const
{
	using namespace std::chrono;
	return duration<float>(high_resolution_clock::now() - start).count();
}

float Time::GetFPS() const
{
	return 1 / deltaTime;
}
