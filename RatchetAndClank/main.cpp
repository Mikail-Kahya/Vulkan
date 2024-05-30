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
	Scene& scene = SceneManager::GetInstance().LoadScene("Triangle");

	GameObject* ratchet =  scene.SpawnObject("Ratchet");
	MeshComponent* meshCompPtr = ratchet->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh("Ratchet/Ratchet.obj");
	meshCompPtr->SetTexture("Ratchet/Ratchet.png");
	meshCompPtr->SetShader(shader3DName, shader3DName);
	ratchet->SetLocalScale({ 2.5f, 2.5f, 2.5f });

    GameObject* playerCamera = scene.SpawnObject("Camera");
    playerCamera->SetParent(ratchet);
    playerCamera->SetLocalPosition({ 0, 2.f, 10.f });
    CameraComponent* cameraCompPtr = playerCamera->AddComponent<CameraComponent>();
    cameraCompPtr->Activate();


	GameObject* tuktuk = scene.SpawnObject("tuktuk");
	meshCompPtr = tuktuk->AddComponent<MeshComponent>();
	meshCompPtr->SetMesh("tuktuk.obj");
	meshCompPtr->SetTexture("tuktuk.png");
	meshCompPtr->SetShader(shader3DName, shader3DName);

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
    commandInput->AddBinding(id, forward, commandInput->AddCommand<MoveCommand>(ratchet, glm::vec3{0, 0, 1}, 10.f));
    commandInput->AddBinding(id, back, commandInput->AddCommand<MoveCommand>(ratchet, glm::vec3{0, 0, -1}, 10.f));
    commandInput->AddBinding(id, left, commandInput->AddCommand<MoveCommand>(ratchet, glm::vec3{-1, 0, 0}, 10.f));
    commandInput->AddBinding(id, right, commandInput->AddCommand<MoveCommand>(ratchet, glm::vec3{1, 0, 0}, 10.f));
	commandInput->AddDirectionalBinding(id, look, commandInput->AddCommand<RotateCommand>(ratchet, 50.f));

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
