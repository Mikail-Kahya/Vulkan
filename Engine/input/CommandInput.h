#pragma once
#include <memory>

#include "IInput.h"
#include "Action.h"
#include "Command.h"

namespace mk
{
	class CommandInput : public IInput
	{
	public:
		virtual ~CommandInput()									= default;
		CommandInput(const CommandInput& other)					= delete;
		CommandInput(CommandInput&& other) noexcept				= delete;
		CommandInput& operator=(const CommandInput& other)		= delete;
		CommandInput& operator=(CommandInput&& other) noexcept	= delete;

		virtual void AddBinding(controller_id id, const Action& action, Command* commandPtr) = 0;

		template<std::derived_from<Command> CommandType, typename... Args>
		CommandType* AddCommand(const Args&... args)
		{
			std::unique_ptr<CommandType> command{ std::make_unique<CommandType>(args...) };
			CommandType* commandPtr{ command.get() };
			m_Commands.emplace_back(std::move(command));
			return commandPtr;
		}

		void RemoveCommand(Command* commandPtr)
		{
			auto commandIter = std::ranges::find_if(m_Commands, [commandPtr](const std::unique_ptr<Command>& command)
				{
					return command.get() == commandPtr;
				});

			m_Commands.erase(commandIter);
		}

	protected:
		CommandInput() = default;

	private:
		std::vector<std::unique_ptr<Command>> m_Commands;
	};
}
