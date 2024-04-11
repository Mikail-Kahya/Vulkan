#pragma once
#include "glm/glm.hpp"

namespace mk
{
	class Camera final
	{
	public:
		Camera();
		Camera(float width, float height, float fovDeg);
		~Camera()	= default;

		Camera(const Camera& other)					= default;
		Camera(Camera&& other) noexcept				= default;
		Camera& operator=(const Camera& other)		= default;
		Camera& operator=(Camera&& other) noexcept	= default;

		void Update();

		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;

		void SetViewport(float width, float height);
		void SetFOV(float fovDeg);

	private:
		void CalculateProjectionMatrix();
		void CalculateViewMatrix();
		
		// Controls
		//void Move(const uint8_t* keyboardStatePtr, float deltaTime);
		//void Rotate(int mouseX, int mouseY);
		//void MouseMove(int mouseY, float deltaTime, float moveSpeed = 10.f);
		//void VerticalMove(int direction, float deltaTime, float moveSpeed = 10.f);
		
		// Helpers
		void FlagView();
		void FlagProjection();

		glm::mat4 m_ViewMatrix{};
		glm::mat4 m_ProjectionMatrix{};
		glm::vec3 m_Position{2, 2, 2};

		glm::vec3 m_Forward{ 0, 0, 1 };
		glm::vec3 m_Up{ 0, 1, 0 };

		float m_AspectRatio{ 1.6f };
		float m_FOV{};

		float m_ZFar{ 1000.f };
		float m_ZNear{ .1f };

		//float m_TotalPitch{};
		//float m_TotalYaw{};
		//
		//float m_RotateSpeed{ 1.5f };
		//float m_MoveSpeed{ 0.f };

		bool m_FlagProjection{ true };
		bool m_FlagView{ true };
	};
}
