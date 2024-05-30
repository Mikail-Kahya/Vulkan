#pragma once
#include <memory>
#include <vector>

namespace mk
{
	class Command;

	template<typename ActionType, typename CommandType>
	class InputMapping final
	{
	public:
		using Mapping = std::pair<ActionType, CommandType*>;
		using Mappings = std::vector<Mapping>;

		InputMapping() = default;
		~InputMapping() = default;

		InputMapping(const InputMapping& other)					= default;
		InputMapping(InputMapping&& other) noexcept				= default;
		InputMapping& operator=(const InputMapping& other)		= default;
		InputMapping& operator=(InputMapping&& other) noexcept	= default;

		inline void AddMapping(const ActionType& action, CommandType* commandPtr)
		{
			Mapping map{ std::make_pair(action, commandPtr) };
			m_Mappings.emplace_back(map);
		}

		inline void RemoveMapping(CommandType* commandPtr)
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
