#pragma once
#include <optional>

namespace mk
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool IsComplete() const;
	};
}