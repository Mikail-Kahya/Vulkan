#pragma once
#include <memory>
#include <vector>

#include "Controller.h"
#include "Mouse.h"
#include "CommandInput.h"
#include "InputMapping.h"

namespace mk
{
	class Command;

	class GLFWInput final : public CommandInput
	{
	public:
		GLFWInput(GLFWwindow* windowPtr);
		~GLFWInput() override = default;
		GLFWInput(const GLFWInput& other)					= delete;
		GLFWInput(GLFWInput&& other) noexcept				= delete;
		GLFWInput& operator=(const GLFWInput& other)		= delete;
		GLFWInput& operator=(GLFWInput&& other) noexcept	= delete;

		bool ProcessInput() override;
		controller_id RegisterController() override;
		void UnregisterController(controller_id id) override;
		void AddBinding(controller_id id, const Action& action, Command* commandPtr) override;
        void AddDirectionalBinding(controller_id id, const DirectionAction& action, DirectionCommand* commandPtr) override;
		const Mouse* GetMouse() const override;

	private:
		bool ValidController(controller_id id) const;

		uint8_t m_ControllerIdx{};
		std::unique_ptr<Mouse> m_Mouse{};
		std::vector<std::unique_ptr<Controller>> m_Controllers{};
		std::vector<std::unique_ptr<Command>> m_Commands{};
		GLFWwindow* m_WindowPtr{};
	};

}
