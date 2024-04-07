#pragma once
#include <vector>

#include "Structs.h"

namespace mk
{
	class Mesh final
	{
	public:
		Mesh()	= default;
		~Mesh()	= default;

		Mesh(const Mesh& other)					= delete;
		Mesh(Mesh&& other) noexcept				= delete;
		Mesh& operator=(const Mesh& other)		= delete;
		Mesh& operator=(Mesh&& other) noexcept	= delete;

		void AddVertices(const std::vector<Vertex>& vertices);

	private:
		std::vector<Vertex> m_Vertices{};
	};
}
