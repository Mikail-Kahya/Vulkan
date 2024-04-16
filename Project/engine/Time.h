#pragma once
#include <chrono>

namespace mk
{
	class Time final
	{
	public:
		Time()	= default;
		~Time() = default;

		Time(const Time& other) = default;
		Time(Time&& other) noexcept = default;
		Time& operator=(const Time& other) = default;
		Time& operator=(Time&& other) noexcept = default;

		static void Update();
		static float DeltaTime();

	private:
		inline static std::chrono::high_resolution_clock::time_point m_PrevTime{};
		inline static float m_DeltaTime{};
	};
}
