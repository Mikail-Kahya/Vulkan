#include "VulkanStructs.h"

using namespace mk;

bool QueueFamilyIndices::IsComplete() const
{
	return graphicsFamily.has_value();
}
