#pragma once
#include <memory>
#include <vector>

#include "Action.h"
#include "Command.h"

namespace mk
{
	class Command;

	class InputMapping final
	{
	public:
		using Mapping = std::pair<Action, Command*>;
		using Mappings = std::vector<Mapping>;

		InputMapping() = default;
		~InputMapping() = default;

		InputMapping(const InputMapping& other)					= default;
		InputMapping(InputMapping&& other) noexcept				= default;
		InputMapping& operator=(const InputMapping& other)		= default;
		InputMapping& operator=(InputMapping&& other) noexcept	= default;

		inline void AddMapping(const Action& action, Command* commandPtr)
		{
			Mapping map{ std::make_pair(action, commandPtr) };
			m_Mappings.emplace_back(map);
		}

		inline void RemoveMapping(Command* commandPtr)
		{
			std::erase_if(m_Mappings, [commandPtr](const Mapping& mapping)
				{
					return mapping.second == commandPtr;
				});
		}

		inline const Mappings& GetMappings() const { return m_Mappings; }

	private:
		Mappings m_Mappings{};
	};
}
