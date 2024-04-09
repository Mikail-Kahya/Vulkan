#include "Mesh.h"

using namespace mk;

Mesh::~Mesh()
{
	m_VertexBuffer.Destroy();
}

void Mesh::Draw(VkCommandBuffer commandBuffer) const
{
	m_VertexBuffer.Draw(commandBuffer);
}

void Mesh::Load(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	m_VertexBuffer.SetVertices(vertices);
	m_VertexBuffer.SetIndices(indices);
}
