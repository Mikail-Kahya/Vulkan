#pragma once
#include <chrono>

namespace mk
{
	struct Time
	{
		Time();

		Time(const Time& other) = default;
		Time(Time&& other) = default;
		Time& operator=(const Time& other) = default;
		Time& operator=(Time&& other) = default;

		float GetTotalTime() const;
		float GetFPS() const;

		float deltaTime{};
		float fixedDeltaTime{ 0.02f };

	private:
		std::chrono::steady_clock::time_point start{};
	};
}