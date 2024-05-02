#include "Mesh2D.h"

using namespace mk;

void Mesh2D::Draw(VkCommandBuffer commandBuffer) const
{
	m_VertexBuffer.Draw(commandBuffer);
}

void Mesh2D::Load(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	m_VertexBuffer.SetVertices(vertices);
	m_VertexBuffer.SetIndices(indices);
}
