#include <iostream>

#include "core/ResourceManager.h"
#include "core/Scene.h"
#include "core/SceneManager.h"
#include "vulkan/VulkanBase.h"

using namespace mk;

void Load()
{
	std::vector<Vertex2D> vertices = {
	{{-0.8f, 0.5f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.8f}, {0.0f, 0.0f, 1.0f}},
	{{-0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<Vertex3D> vertices3D = {
	{{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}, {0, 0}},
	{{0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}, {1, 0}},
	{{0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}, {1, 1}},
	{{-0.5f, 0.5f, 0.f}, {1.0f, 1.0f, 1.0f}, {0, 1}}
	};

	const std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	const std::string shader2DName{ "shader2D" };
	const std::string shader3DName{ "shader3D" };
	const std::string texture{ "test.jpg" };
	//Scene* scenePtr = SceneManager::GetInstance().LoadScene("Triangle");
	//
	//Mesh3D* plane = scenePtr->AddMesh3D(shader3DName, texture);
	//plane->Load(vertices3D, indices);
	//plane->SetPosition({ -1, 0, 0 });
	//
	//Mesh3D* plane2 = scenePtr->AddMesh3D(shader3DName, texture);
	//plane2->Load(vertices3D, indices);
	//plane2->SetPosition({ 1, 0, 0 });
	//
	//Mesh2D* plane2D = scenePtr->AddMesh2D(shader2DName);
	//plane2D->Load(vertices, indices);
	//
	//Mesh2D* plane2D2 = scenePtr->AddMesh2D(shader2DName);
	//for (auto& vertex : vertices)
	//	vertex.pos *= -1;
	//plane2D2->Load(vertices, indices);

	//Mesh3D* tuktuk = scenePtr->AddMesh3D(shader3DName, "tuktuk.png");
	//tuktuk->Load("tuktuk.obj");

	//Mesh3D* ratchet = scenePtr->AddMesh3D(shader3DName, "Ratchet/Ratchet.png");
	//ratchet->Load("Ratchet/Ratchet.obj");
	//ratchet->SetScale({ 2.5f, 2.5f, 2.5f });

	//Mesh3D* clank = scenePtr->AddMesh3D(shader3DName, "Clank/ClankBackpack.png");
	//clank->Load("Clank/ClankBackpack.obj");
	//clank->SetPosition({ 0, 1, -1 });
	//clank->SetScale({ 0.1f, 0.1f, 0.1f });


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

	ResourceManager::SetDefaultShaderPath("shaders/");
	ResourceManager::SetDefaultTexturePath("resources/");
	ResourceManager::SetDefaultMeshPath("resources/");

	try {
		//VulkanBase::GetInstance().Run(&Load);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}