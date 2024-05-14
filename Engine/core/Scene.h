#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "Mesh2D.h"
#include "Mesh3D.h"

namespace mk
{
	class Pipeline3D;
	class Pipeline2D;

	class Scene final
	{
		using Meshes2D = std::vector<std::unique_ptr<Mesh2D>>;
		using Meshes3D = std::vector<std::unique_ptr<Mesh3D>>;
	public:
		Scene()		= default;
		~Scene()	= default;

		Scene(const Scene& other)					= delete;
		Scene(Scene&& other) noexcept				= delete;
		Scene& operator=(const Scene& other)		= delete;
		Scene& operator=(Scene&& other) noexcept	= delete;

		void Draw() const;

		Mesh2D* AddMesh2D(const std::string& shader);
		Mesh3D* AddMesh3D(const std::string& shader, const std::string& texture);
		void RemoveMesh(Mesh2D* meshPtr);
		void RemoveMesh(Mesh3D* meshPtr);

	private:
		std::unordered_map<Pipeline2D*, Meshes2D> m_Mesh2DSets{};
		std::unordered_map<Pipeline3D*, Meshes3D> m_Mesh3DSets{};
	};
}
