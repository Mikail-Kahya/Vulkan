#pragma once
#include <vector>

#include "Structs.h"
#include "VertexBuffer.h"

namespace mk
{
	class Mesh final
	{
	public:
		Mesh()	= default;
		~Mesh();

		Mesh(const Mesh& other)					= delete;
		Mesh(Mesh&& other) noexcept				= delete;
		Mesh& operator=(const Mesh& other)		= delete;
		Mesh& operator=(Mesh&& other) noexcept	= delete;

		void Draw(VkCommandBuffer commandBuffer) const;
		void Load(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

	private:
		VertexBuffer m_VertexBuffer{};
	};
}
