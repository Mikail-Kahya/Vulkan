#include "GLFWInput.h"

#include "Controller.h"

using namespace mk;

GLFWInput::GLFWInput(GLFWwindow* windowPtr)
	: m_Mouse{ std::make_unique<Mouse>(windowPtr) }
	, m_WindowPtr{ windowPtr }
{
}

bool GLFWInput::ProcessInput()
{
	if (glfwWindowShouldClose(m_WindowPtr))
		return false;

	for (Controller& controller : m_Controllers)
	{
		for (const auto& mapping : controller.GetInputMapping().GetMappings())
		{
			const KeyInput key{ mapping.first.keyboardInput };
			if (glfwGetKey(m_WindowPtr, key))
				controller.KeyUp(key);
			else
				controller.KeyDown(key);
		}
		controller.HandleInput();
	}

	return true;
}


controller_id GLFWInput::RegisterController()
{
	// Reuse old controllers
	const auto foundInvalidController =
		std::find_if(m_Controllers.begin(), m_Controllers.end(),
			[this](const Controller& controller)
			{
				return !ValidController(controller.GetIdx());
			});

	if (foundInvalidController == m_Controllers.end())
	{
		m_Controllers.emplace_back(m_ControllerIdx, 1000, true);
		return ++m_ControllerIdx;
	}

	foundInvalidController->Enable();
	return foundInvalidController->GetIdx();
}

void GLFWInput::UnregisterController(controller_id id)
{
	if (!ValidController(id))
		return;

	m_Controllers[id].Disable();
}

void GLFWInput::AddBinding(controller_id id, const Action& action, Command* commandPtr)
{
	if (!ValidController(id))
		return;

	m_Controllers[id].AddBinding(action, commandPtr);
}

bool GLFWInput::ValidController(controller_id id) const
{
	if (id >= m_ControllerIdx)
		return false;

	return m_Controllers[id].IsEnabled();
}
