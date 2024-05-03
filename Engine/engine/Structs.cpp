#include "Structs.h"

using namespace mk;

VkVertexInputBindingDescription* Vertex2D::GetBindingDescription()
{
	static VkVertexInputBindingDescription bindingDescription{};

	if (bindingDescription.stride != sizeof(Vertex2D))
	{
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex2D);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	}
	
	return &bindingDescription;
}

std::vector<VkVertexInputAttributeDescription>* Vertex2D::GetAttributeDescriptions()
{
	static std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	if (attributeDescriptions.empty())
	{
		attributeDescriptions.resize(2);

		// Pos
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex2D, pos);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex2D, color);

		// texCoord
		//attributeDescriptions[2].binding = 0;
		//attributeDescriptions[2].location = 2;
		//attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		//attributeDescriptions[2].offset = offsetof(Vertex2D, texCoord);
	}

	return &attributeDescriptions;
}

VkVertexInputBindingDescription* Vertex3D::GetBindingDescription()
{
	static VkVertexInputBindingDescription bindingDescription{};

	if (bindingDescription.stride != sizeof(Vertex3D))
	{
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex3D);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	}

	return &bindingDescription;
}

std::vector<VkVertexInputAttributeDescription>* Vertex3D::GetAttributeDescriptions()
{
	static std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	if (attributeDescriptions.empty())
	{
		attributeDescriptions.resize(3);

		// Pos
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex3D, pos);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex3D, color);

		// texCoord
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex3D, texCoord);
	}

	return &attributeDescriptions;
}
