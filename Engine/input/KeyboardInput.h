#pragma once
#include <cstdint>
#include <vector>

namespace mk
{
	using KeyInput = int32_t;

	class KeyboardInput final
	{
	public:
		KeyboardInput(KeyInput nrKeys);
		~KeyboardInput() = default;

		KeyboardInput(const KeyboardInput& other)					= delete;
		KeyboardInput(KeyboardInput&& other) noexcept				= delete;
		KeyboardInput& operator=(const KeyboardInput& other)		= delete;
		KeyboardInput& operator=(KeyboardInput&& other) noexcept	= delete;

		void Update();
		void PollButton(KeyInput button, bool isPressed);

		bool ButtonDown(KeyInput key) const;
		bool ButtonHold(KeyInput key) const;
		bool ButtonUp(KeyInput key) const;

	private:
		std::vector<bool> m_PrevKeys{};
		std::vector<bool> m_CurrentKeys{};
	};
}
