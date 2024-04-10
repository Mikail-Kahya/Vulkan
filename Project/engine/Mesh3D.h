#pragma once
#include <vector>

#include "Structs.h"
#include "VertexBuffer3D.h"

namespace mk
{
	class Mesh3D final
	{
		using VertexBuffer = VertexBuffer3D;
		using Vertex = Vertex3D;
	public:
		Mesh3D()	= default;
		~Mesh3D()	= default;

		Mesh3D(const Mesh3D& other)					= delete;
		Mesh3D(Mesh3D&& other) noexcept				= delete;
		Mesh3D& operator=(const Mesh3D& other)		= delete;
		Mesh3D& operator=(Mesh3D&& other) noexcept	= delete;

		void Draw(VkCommandBuffer commandBuffer) const;
		void Load(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

	private:
		VertexBuffer m_VertexBuffer{};
	};
}
