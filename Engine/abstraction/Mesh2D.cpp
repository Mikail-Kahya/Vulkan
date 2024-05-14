#include "Mesh2D.h"

#include "core/Utils.h"

using namespace mk;

void Mesh2D::Draw(VkCommandBuffer commandBuffer) const
{
	m_VertexBuffer.Draw(commandBuffer);
}

void Mesh2D::Load(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_VertexBuffer.SetVertices(vertices);
	m_VertexBuffer.SetIndices(indices);
}

void Mesh2D::Load(const std::string& objFile)
{
	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};
	if (ParseOBJ<Vertex2D>(objFile, vertices, indices))
		Load(vertices, indices);
}


