#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "Mesh.h"

namespace mk
{
	class Pipeline;

	class Scene final
	{
		using Meshes = std::vector<std::unique_ptr<Mesh>>;
	public:
		Scene()		= default;
		~Scene()	= default;

		Scene(const Scene& other)					= delete;
		Scene(Scene&& other) noexcept				= delete;
		Scene& operator=(const Scene& other)		= delete;
		Scene& operator=(Scene&& other) noexcept	= delete;

		void Draw(uint32_t imageIdx) const;

		Mesh* AddMesh(const std::string& shader);
		void RemoveMesh(Mesh* meshPtr);

	private:
		std::unordered_map<Pipeline*, Meshes> m_MeshSets{};
	};
}
