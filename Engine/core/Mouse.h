#pragma once
#include <glm/vec2.hpp>

#include "GLFW/glfw3.h"

namespace mk
{
	class Mouse final
	{
	public:
		Mouse(GLFWwindow* windowPtr);
		~Mouse() = default;

		Mouse(const Mouse& other)					= default;
		Mouse(Mouse&& other) noexcept				= default;
		Mouse& operator=(const Mouse& other)		= default;
		Mouse& operator=(Mouse&& other) noexcept	= default;

		void Update();

		const glm::vec2& GetDirection() const;
		const glm::vec2& GetDisplacement() const;
		glm::vec2 GeScreenPos() const;
		bool LeftClick() const;
		bool RightClick() const;

	private:
		void SetViewport();
		glm::vec2 GetWindowPos() const;
		

		GLFWwindow* m_WindowPtr{ nullptr };
		glm::vec2 m_Center{};
		glm::vec2 m_Displacement{};
		glm::vec2 m_Direction{};
		glm::vec2 m_Position{};
	};
}
