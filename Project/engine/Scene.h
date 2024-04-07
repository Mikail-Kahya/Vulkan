#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Mesh.h"

namespace mk
{
	class Pipeline;

	class Scene final
	{
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
		using AllocatedMesh = std::pair<Pipeline*, std::unique_ptr<Mesh>>;
		std::vector<AllocatedMesh> m_Meshes;
	};
}
