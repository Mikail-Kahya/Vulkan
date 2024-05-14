#pragma once
#include <vector>

#include "core/Structs.h"
#include "abstraction/VertexBuffer.h"

namespace mk
{
	using Vertex = Vertex2D;
	class Mesh2D final
	{
	public:
		Mesh2D()	= default;
		~Mesh2D()	= default;

		Mesh2D(const Mesh2D& other)					= delete;
		Mesh2D(Mesh2D&& other) noexcept				= delete;
		Mesh2D& operator=(const Mesh2D& other)		= delete;
		Mesh2D& operator=(Mesh2D&& other) noexcept	= delete;

		void Draw(VkCommandBuffer commandBuffer) const;
		void Load(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		void Load(const std::string& objFile);

	private:
		VertexBuffer m_VertexBuffer{};
	};
}
