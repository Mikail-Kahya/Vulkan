#pragma once
#include "vulkan/vulkan_core.h"
#include "glm/glm.hpp"

#include <vector>

namespace mk
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription* GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
	};
}
