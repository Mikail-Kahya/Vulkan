#include "Structs.h"

using namespace mk;

VkVertexInputBindingDescription* Vertex::GetBindingDescription()
{
	static VkVertexInputBindingDescription bindingDescription{};

	if (bindingDescription.stride != sizeof(Vertex))
	{
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	}
	
	return &bindingDescription;
}

std::vector<VkVertexInputAttributeDescription>* Vertex::GetAttributeDescriptions()
{
	static std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
	if (attributeDescriptions.empty())
	{
		attributeDescriptions.resize(2);

		// Pos
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
	}

	return &attributeDescriptions;
}
