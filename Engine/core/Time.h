#pragma once
#include <chrono>

namespace mk
{
	class Time final
	{
	public:
		Time(const Time& other)					= default;
		Time(Time&& other) noexcept				= default;
		Time& operator=(const Time& other)		= default;
		Time& operator=(Time&& other) noexcept	= default;

		static void Update();
		static float GetTotalTime();
		static float GetFPS();
		static float GetDeltaTime();

		constexpr static float FIXED_TIME{ 0.02f };

	private:
		Time() = default;

		inline static float m_DeltaTime{};
		static std::chrono::high_resolution_clock::time_point m_Start;
		static std::chrono::high_resolution_clock::time_point m_LastUpdated;
	};
}