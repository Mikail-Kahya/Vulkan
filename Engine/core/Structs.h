#pragma once
#include "vulkan/vulkan_core.h"
#include "glm/glm.hpp"

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace mk
{
	struct Vertex2D
	{
		glm::vec2 pos;
		glm::vec3 color;
		//glm::vec2 texCoord;
		//glm::vec3 normal;
		//glm::vec3 tangent;

		bool operator==(const Vertex2D& other) const;
		

		static VkVertexInputBindingDescription* GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
	};

	struct Vertex3D
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;
		glm::vec3 normal;
		glm::vec3 tangent;

		bool operator==(const Vertex3D& other) const;


		static VkVertexInputBindingDescription* GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
	};
}

namespace std {
	template<> struct hash<mk::Vertex3D> {
		size_t operator()(mk::Vertex3D const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
					(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
					(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};

	template<> struct hash<mk::Vertex2D> {
		size_t operator()(mk::Vertex2D const& vertex) const {
			return ((hash<glm::vec2>()(vertex.pos) ^
					(hash<glm::vec3>()(vertex.color) << 1)) >> 1);
		}
	};
}