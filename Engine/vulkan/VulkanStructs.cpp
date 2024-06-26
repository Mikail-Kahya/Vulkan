#include "VulkanStructs.h"

using namespace mk;

bool QueueFamilyIndices::IsComplete() const
{
	return graphicsFamily.has_value() && presentFamily.has_value();
}

bool SwapChainSupportDetails::IsComplete() const
{
	return !presentModes.empty() && !formats.empty();
}
