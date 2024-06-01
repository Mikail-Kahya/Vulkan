#include <array>
#include <iostream>

#include <components/cameracomponent.h>

#include <core/ServiceLocator.h>
#include <input/CommandInput.h>
#include <input/InputMapping.h>

#include "MkVulktra.h"
#include "input/GLFWInput.h"
#include "playercommands.h"
#include "core/ResourceManager.h"
#include "core/Scene.h"
#include "core/SceneManager.h"
#include "input/Action.h"

#include "components/MeshComponent.h"
#include "components/SpriteComponent.h"
#include "components/StateComponent.h"

#include "SaucerStates.h"
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
	constexpr float speed{ 20.f };
	commandInput->AddBinding(id, forward, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ 0, 0, 1 }, speed));
	commandInput->AddBinding(id, back, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ 0, 0, -1 }, speed));
	commandInput->AddBinding(id, left, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ -1, 0, 0 }, speed));
	commandInput->AddBinding(id, right, commandInput->AddCommand<MoveCommand>(player, glm::vec3{ 1, 0, 0 }, speed));
	commandInput->AddDirectionalBinding(id, look, commandInput->AddCommand<RotateCommand>(player, glm::vec2{ 5.5f, 0 }));

	return player;
}

void CreateEnvironment(Scene& scene, const std::string& shader)
{
	const std::string dome{ "environment/skydome.obj" };
	ResourceManager::LoadMesh3D(dome, true);

	GameObject* sky = scene.SpawnObject("Sky");
	sky->SetLocalScale(glm::vec3{ 800.f });
	MeshComponent* meshCompPtr = sky->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh(dome);
	meshCompPtr->SetTexture("environment/space.png");
	meshCompPtr->SetShader(shader, shader);

	GameObject* floor = scene.SpawnObject("Floor");
	floor->SetLocalScale(glm::vec3{ 800.f });
	meshCompPtr = floor->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh("environment/ground.obj");
	meshCompPtr->SetTexture("environment/rock.jpg");
	meshCompPtr->SetShader(shader, shader);

	GameObject* planets = scene.SpawnObject("planets");
	planets->SetLocalPosition(glm::vec3{ 500.f, 50.f, 100.f });
	planets->SetLocalScale(glm::vec3{ 20.f });
	meshCompPtr = planets->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh("environment/planets.obj");
	meshCompPtr->SetShader(shader, shader+"Color");

	constexpr int nrRockModels{ 2 };
	constexpr int nrRocks{ 5 };
	const std::array<std::string, nrRockModels> rocks{ "rock1", "rock2" };
	constexpr int limitX{ 500 };
	constexpr int limitZ{ 500 };
	constexpr int limitScale{ 10 };
	for (int idx{}; idx < nrRocks; ++idx)
	{
		const std::string& rockModel{ rocks[rand() % nrRockModels] };
		GameObject* rock = scene.SpawnObject(rockModel);
		rock->SetLocalPosition(glm::vec3{ 
			rand() % limitX - limitX * 0.5f,
			-10.f,
			rand() % limitZ - limitZ * 0.5f
		});
		rock->SetLocalScale(glm::vec3{ rand() % limitScale + limitScale * 0.5f });
		meshCompPtr = rock->AddComponent<MeshComponent>();
		meshCompPtr->SetMesh("environment/" + rockModel + ".obj");
		meshCompPtr->SetTexture("environment/rock.jpg");
		meshCompPtr->SetShader(shader, shader);
	}
}

void CreateMovingSpacePlane(Scene& scene, const std::string& shader)
{
	GameObject* saucer = scene.SpawnObject("saucer");
	saucer->AddLocalOffset({ 200.f, 0.f, 200.f });
	MeshComponent* meshCompPtr = saucer->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh("saucer/Saucer.obj");
	meshCompPtr->SetTexture("saucer/Saucer.png");
	meshCompPtr->SetShader(shader, shader);

	StateComponent* stateCompPtr = saucer->AddComponent<StateComponent>("Land", std::make_unique<LandingState>());
	stateCompPtr->AddState("Hover", std::make_unique<FlyingState>());
}

void LoadSecondPipeline(Scene& scene)
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
	const std::string shape{ "shape" };
	ResourceManager::LoadMesh2D(shape, vertices, indices);

	GameObject* shapeObj = scene.SpawnObject("Shape");
	shapeObj->SetLocalScale(glm::vec3{ 800.f });
	SpriteComponent* spriteCompPtr = shapeObj->AddComponent<SpriteComponent>();
	spriteCompPtr->SetMesh(shape);
	spriteCompPtr->SetShader(shader2DName, shader2DName);
}

void Load()
{
	
	const std::string shader3DName{ "shader3D" };
	Scene& scene = SceneManager::GetInstance().LoadScene("RatchetTest");

	AddClank(scene, CreatePlayer(scene, shader3DName), shader3DName);
	CreateEnvironment(scene, shader3DName);
	CreateMovingSpacePlane(scene, shader3DName);
	LoadSecondPipeline(scene);
	

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

	srand(time(0));
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
