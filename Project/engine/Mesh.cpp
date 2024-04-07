#include "Mesh.h"

using namespace mk;

void Mesh::AddVertices(const std::vector<Vertex>& vertices)
{
	m_Vertices.insert(m_Vertices.begin(), vertices.begin(), vertices.end());
}
