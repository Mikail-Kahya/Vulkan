#pragma once
#include <functional>
#include <chrono>

namespace mk
{
	class MkVulktra final
	{
	public:
		MkVulktra();
		~MkVulktra();

		void Run(const std::function<void()>& load);
		void RunOneFrame();

		MkVulktra(const MkVulktra& other) = delete;
		MkVulktra(MkVulktra&& other) = delete;
		MkVulktra& operator=(const MkVulktra& other) = delete;
		MkVulktra& operator=(MkVulktra&& other) = delete;

	private:
		bool m_quit{};
		std::chrono::high_resolution_clock::time_point m_LastTime{};

		static constexpr int FPS{ 60 };
		static constexpr float FIXED_TIME_STEP{ 0.02f };

		float m_Lag{};
	};
}