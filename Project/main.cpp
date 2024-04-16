#include <iostream>

#include "engine/Scene.h"
#include "engine/SceneManager.h"
#include "vulkanbase/VulkanBase.h"

void Load()
{
	std::vector<mk::Vertex2D> vertices = {
	{{-0.8f, 0.5f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.8f}, {0.0f, 0.0f, 1.0f}},
	{{-0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<mk::Vertex3D> vertices3D = {
	{{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	const std::string shader2DName{ "shader2D" };
	const std::string shader3DName{ "shader3D" };
	mk::Scene* scenePtr = mk::SceneManager::GetInstance().LoadScene("Triangle");

	mk::Mesh3D* plane = scenePtr->AddMesh3D(shader3DName);
	plane->Load(vertices3D, indices);
	plane->SetPosition({ -1, 0, 0 });

	mk::Mesh3D* plane2 = scenePtr->AddMesh3D(shader3DName);
	plane2->Load(vertices3D, indices);
	plane2->SetPosition({ 1, 0, 0 });

	mk::Mesh2D* plane2D = scenePtr->AddMesh2D(shader2DName);
	plane2D->Load(vertices, indices);

	mk::Mesh2D* plane2D2 = scenePtr->AddMesh2D(shader2DName);
	for (auto& vertex : vertices)
		vertex.pos *= -1;
	plane2D2->Load(vertices, indices);

	mk::Mesh3D* tuktuk = scenePtr->AddMesh3D(shader3DName);
	tuktuk->Load("tuktuk.obj");


	std::cout << "Controls:\n\n"
		<< "=========\n\n"
		<< "Rotation:\n"
		<< "---------\n"
		<< "Hold right click to rotate camera:\n\n"
		<< "=========\n\n"
		<< "Move:\n"
		<< "-----\n"
		<< "W => Move forward\n"
		<< "S => Move backwards \n"
		<< "D => Move right \n"
		<< "A => Move left \n";
}

int main() {
	// DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1 = 1
	//DISABLE_LAYER_NV_OPTIMUS_1 = 1
	//_putenv_s("DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1", "1");
	//_putenv_s("DISABLE_LAYER_NV_OPTIMUS_1", "1");

	try {
		mk::VulkanBase::GetInstance().Run(&Load);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}