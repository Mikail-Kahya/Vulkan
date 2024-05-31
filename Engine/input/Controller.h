
#pragma once
#include "ControllerInput.h"
#include "KeyboardInput.h"
#include "InputMapping.h"

#include "Action.h"
#include "Command.h"

namespace mk
{
	class Controller final
	{
		using DefaultInputMapping = InputMapping<Action, Command>;
		using DirectionInputMapping = InputMapping<DirectionAction, DirectionCommand>;

	public:
		Controller(uint8_t idx, KeyInput nrKeys, bool useKeyboard = false);
		~Controller() = default;

		Controller(const Controller& other)					= delete;
		Controller(Controller&& other) noexcept				= delete;
		Controller& operator=(const Controller& other)		= delete;
		Controller& operator=(Controller&& other) noexcept	= delete;

		void KeyDown(KeyInput input);
		void KeyUp(KeyInput input);
		void HandleInput();

		uint8_t GetIdx() const;
		const DefaultInputMapping& GetInputMapping() const;
		const DirectionInputMapping& GetDirectionInputMapping() const;

		void AddBinding(const Action& action, Command* commandPtr);
		void RemoveBinding(Command* commandPtr);

		void AddDirectionBinding(const DirectionAction& action, DirectionCommand* commandPtr);
		void RemoveDirectionBinding(DirectionCommand* commandPtr);

		void ClearBindings();

		void Enable();
		void Disable();
		bool IsEnabled() const;

	private:
		DefaultInputMapping m_InputMapping{};
		DirectionInputMapping m_DirectionInputMapping{};
		ControllerInput m_Controller;
		KeyboardInput m_Keyboard;
		uint8_t m_Idx{};
		bool m_UseKeyboard{};
		bool m_Enabled{ false };
	};
}
