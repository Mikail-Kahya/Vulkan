#include <iostream>

#include <components/cameracomponent.h>

#include <core/ServiceLocator.h>
#include <input/CommandInput.h>
#include <input/InputMapping.h>

#include "MkVulktra.h"
#include "input/GLFWInput.h"
#include "playercommands.h"
#include "components/MeshComponent.h"
#include "core/ResourceManager.h"
#include "core/Scene.h"
#include "core/SceneManager.h"
#include "input/Action.h"

using namespace mk;

GameObject* CreateRatchetPart(Scene& scene, GameObject* ratchet, const std::string& part, const std::string& shader)
{
	GameObject* ratchetMesh = scene.SpawnObject(part);
	ratchetMesh->SetParent(ratchet);
	
	MeshComponent* meshCompPtr = ratchetMesh->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh("Ratchet/" + part + ".obj");
	meshCompPtr->SetTexture("Ratchet/" + part + ".png");
	meshCompPtr->SetShader(shader, shader);
	return ratchetMesh;
}

void AddClank(Scene& scene, GameObject* ratchet, const std::string& shader)
{
	GameObject* clank = scene.SpawnObject("Clank");
	clank->SetParent(ratchet);
	clank->SetLocalPosition({ 0, 16.f, -5.f });
	clank->SetLocalRotation({ 0, 180.f, 0 });
	clank->SetLocalScale({ 0.1f, 0.1f, 0.1f });
	MeshComponent* meshCompPtr = clank->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh("Clank/ClankBackpack.obj");
	meshCompPtr->SetTexture("Clank/ClankBackpack.png");
	meshCompPtr->SetShader(shader, shader);
}

GameObject* CreatePlayer(Scene& scene, const std::string& shader)
{
	GameObject* player = scene.SpawnObject("Player");

	GameObject* ratchet = scene.SpawnObject("RatchetWrapper");
	ratchet->SetLocalRotation({ 0, -90.f, 0 });
	ratchet->SetLocalScale({ 3.f, 3.f, 3.f });
	ratchet->SetParent(player);

	CreateRatchetPart(scene, ratchet, "RatchetBody", shader);
	CreateRatchetPart(scene, ratchet, "Watch", shader);
	CreateRatchetPart(scene, ratchet, "Pants", shader);
	CreateRatchetPart(scene, ratchet, "Hat", shader);



	// Camera
	GameObject* playerCamera = scene.SpawnObject("Camera");
	playerCamera->SetParent(player);
	//playerCamera->SetLocalPosition({ 0, 5.f, -40.f });
	playerCamera->SetLocalPosition({ 0, 5.f, -25.f });
	//playerCamera->SetLocalRotation({ 0, 180, 0 });
	CameraComponent* cameraCompPtr = playerCamera->AddComponent<CameraComponent>();
	cameraCompPtr->Activate();

	CommandInput* commandInput = dynamic_cast<CommandInput*>(&ServiceLocator::GetInputSystem());
	const controller_id id = commandInput->RegisterController();

	// controls
	Action forward{};
	forward.controllerInput = Input::dPadUp;
	forward.keyboardInput = GLFW_KEY_W;
	forward.type = ActionType::hold;

	Action back{};
	back.controllerInput = Input::dPadDown;
	back.keyboardInput = GLFW_KEY_S;
	back.type = ActionType::hold;

	Action left{};
	left.controllerInput = Input::dPadLeft;
	left.keyboardInput = GLFW_KEY_A;
	left.type = ActionType::hold;

	Action right{};
	right.controllerInput = Input::dPadRight;
	right.keyboardInput = GLFW_KEY_D;
	right.type = ActionType::hold;

	DirectionAction look{};
	look.inputType = DirectionAction::InputType::mouse;

	// input
	commandInput->AddBinding(id, forward, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ 0, 0, 1 }, 10.f));
	commandInput->AddBinding(id, back, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ 0, 0, -1 }, 10.f));
	commandInput->AddBinding(id, left, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ -1, 0, 0 }, 10.f));
	commandInput->AddBinding(id, right, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ 1, 0, 0 }, 10.f));
	commandInput->AddDirectionalBinding(id, look, commandInput->AddCommand<RotateCommand>(player, glm::vec2{ 5.5f, 0 }));

	return player;
}

void CreateEnvironment(Scene& scene, const std::string& shader)
{
	
}

void Load()
{
	std::vector<Vertex2D> vertices = {
	{{-0.8f, 0.5f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.8f}, {0.0f, 0.0f, 1.0f}},
	{{-0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	const std::string shader2DName{ "shader2D" };
	const std::string shader3DName{ "shader3D" };
	const std::string texture{ "test.jpg" };
	Scene& scene = SceneManager::GetInstance().LoadScene("Triangle");

	AddClank(scene, CreatePlayer(scene, shader3DName), shader3DName);

	

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
		MkVulktra engine{};
		engine.Run(&Load);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
