#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

namespace mk
{
	class MkUltra final
	{
	public:
		explicit MkUltra(const std::filesystem::path& dataPath);
		~MkUltra();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		MkUltra(const MkUltra& other) = delete;
		MkUltra(MkUltra&& other) = delete;
		MkUltra& operator=(const MkUltra& other) = delete;
		MkUltra& operator=(MkUltra&& other) = delete;

	private:
		bool m_quit{};
		std::chrono::high_resolution_clock::time_point m_LastTime{};

		static constexpr int FPS{ 60 };
		static constexpr float FIXED_TIME_STEP{ 0.02f };

		float m_Lag{};
	};
}