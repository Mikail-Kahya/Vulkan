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

void Mesh::Load(const std::vector<Vertex>& vertices)
{
	m_VertexBuffer.SetVertices(vertices);
}
