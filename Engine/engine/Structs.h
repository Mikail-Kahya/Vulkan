#pragma once
#include "vulkan/vulkan_core.h"
#include "glm/glm.hpp"

#include <vector>

namespace mk
{
	struct Vertex2D
	{
		glm::vec2 pos;
		glm::vec3 color;
		glm::vec2 texCoord;
		//glm::vec3 normal;
		//glm::vec3 tangent;
		

		static VkVertexInputBindingDescription* GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
	};

	struct Vertex3D
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;
		//glm::vec3 normal;
		//glm::vec3 tangent;


		static VkVertexInputBindingDescription* GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
	};

}
