#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"


#include "Structs.h"
#include "tiny_obj_loader.h"


namespace mk
{
	struct Vertex3D;
	struct Vertex2D;

	template<typename VertexType>
	concept is_vertex = (
		std::same_as<VertexType, Vertex2D> ||
		std::same_as<VertexType, Vertex3D>
		);
	template<is_vertex VertexType>
	bool ParseOBJ(const std::string& filename, std::vector<VertexType>& vertices, std::vector<uint32_t>& indices, bool flipAxisAndWinding = true)
	{
		tinyobj::attrib_t attrib{};
		std::vector<tinyobj::shape_t> shapes{};
		std::vector<tinyobj::material_t> materials{};
		std::string warn{};
		std::string err{};
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
			throw std::runtime_error(warn + err);

		std::unordered_map<VertexType, uint32_t> uniqueVertices{};

		for (const auto& shape : shapes) 
		{
			for (const auto& index : shape.mesh.indices) {
				// Fill vertex
				VertexType vertex{};

				constexpr int idx0{ 0 };
				constexpr int idx1{ 1 };
				constexpr int idx2{ 2 };
		
				vertex.color = {
					 attrib.colors[3 * index.vertex_index + idx0],
					 attrib.colors[3 * index.vertex_index + idx1],
					 attrib.colors[3 * index.vertex_index + idx2]
				};

				if constexpr (std::same_as<VertexType, Vertex2D>)
				{
					vertex.pos = {	attrib.vertices[3 * index.vertex_index + idx0],
									attrib.vertices[3 * index.vertex_index + idx1] };
					
				}
				else
				{
					vertex.pos = {	attrib.vertices[3 * index.vertex_index + idx0],
									attrib.vertices[3 * index.vertex_index + idx1],
									attrib.vertices[3 * index.vertex_index + idx2] };
		
					vertex.texCoord = {	attrib.texcoords[2 * index.texcoord_index + idx0],
										1.0f - attrib.texcoords[2 * index.texcoord_index + idx1] };

					vertex.normal = {	attrib.normals[3 * index.normal_index + idx0],
										attrib.normals[3 * index.normal_index + idx1],
										attrib.normals[3 * index.normal_index + idx2] };
				}
		
				if (uniqueVertices.count(vertex) == 0) 
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
		
				indices.push_back(uniqueVertices[vertex]);
			}
		}

		if (flipAxisAndWinding)
		{
			for (int idx{}; idx < indices.size(); idx += 3)
			{
				uint32_t temp{ indices[idx + 1] };
				indices[idx + 1] = indices[idx + 2];
				indices[idx + 2] = temp;
			}
		}

		//Cheap Tangent Calculations
		//if constexpr (std::same_as<VertexType, Vertex3D>)
		//{
		//	for (size_t idx{}; idx < vertices.size(); idx += 3)
		//	{
		//		const size_t idx1{ idx + 1 };
		//		const size_t idx2{ idx + 2 };
		//
		//		const glm::vec3& p0 = vertices[idx].pos;
		//		const glm::vec3& p1 = vertices[idx1].pos;
		//		const glm::vec3& p2 = vertices[idx2].pos;
		//		const glm::vec2& uv0 = vertices[idx].texCoord;
		//		const glm::vec2& uv1 = vertices[idx1].texCoord;
		//		const glm::vec2& uv2 = vertices[idx2].texCoord;
		//
		//		const glm::vec3 edge0 = p1 - p0;
		//		const glm::vec3 edge1 = p2 - p0;
		//		const glm::vec3 diffX = glm::vec3(uv1.x - uv0.x, uv2.x - uv0.x, 0);
		//		const glm::vec3 diffY = glm::vec3(uv1.y - uv0.y, uv2.y - uv0.y, 0);
		//		float r = 1.f / glm::cross(diffX, diffY).z;
		//
		//		glm::vec3 tangent = (edge0 * diffY.y - edge1 * diffY.x) * r;
		//		vertices[idx].tangent += tangent;
		//		vertices[idx1].tangent += tangent;
		//		vertices[idx2].tangent += tangent;
		//	}
		//
		//	//Create the Tangents (reject)
		//	for (auto& v : vertices)
		//	{
		//		// calc reject
		//		v.tangent = v.normal * glm::dot(v.tangent, v.normal) / glm::dot(v.normal, v.normal);
		//
		//		if (flipAxisAndWinding)
		//		{
		//			v.pos.z *= -1.f;
		//			v.normal.z *= -1.f;
		//			v.tangent.z *= -1.f;
		//		}
		//
		//	}
		//}
		//
		return true;
	}
}
