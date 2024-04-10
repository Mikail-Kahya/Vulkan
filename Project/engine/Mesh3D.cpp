#include "Mesh3D.h"

using namespace mk;

void Mesh3D::Draw(VkCommandBuffer commandBuffer) const
{
	m_VertexBuffer.Draw(commandBuffer);
}

void Mesh3D::Load(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	m_VertexBuffer.SetVertices(vertices);
	m_VertexBuffer.SetIndices(indices);
}
