#pragma once
#include "Keyboard.h"
#include "Mouse.h"
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

        void Update(const Mouse& mouse, const Keyboard& keyboard); // remove in the future
        void Update();
        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::vec3& rotation);

		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;

		void SetViewport(float width, float height);
		void SetFOV(float fovDeg);

	private:
		void CalculateProjectionMatrix();
		void CalculateViewMatrix();

        void SetYaw(float rotation);
        void SetPitch(float rotation);

		// Controls
        void Move(const Keyboard& keyboard); // remove in the future
        void Rotate(const glm::vec2& mouseDir); // remove in the future

		// Helpers
		void FlagView();
		void FlagProjection();

		glm::mat4 m_ViewMatrix{};
		glm::mat4 m_ProjectionMatrix{};
		glm::vec3 m_Position{ 0, 0, 10 };

		// Currently an issue with right handed vs left handed
		glm::vec3 m_Forward{ 0, 0, -1 };
		glm::vec3 m_Right{ 1, 0, 0 };
		glm::vec3 m_Up{ 0, 1, 0 };
		float m_AspectRatio{ 1.6f };
		float m_FOV{};

		float m_ZFar{ 1000.f };
		float m_ZNear{ 0.1f };

		float m_TotalPitch{};
		float m_TotalYaw{ 180 };
		
        float m_RotateSpeed{ 10.f }; // remove in the future
        float m_MoveSpeed{ 10.f }; // remove in the future

		bool m_FlagProjection{ true };
		bool m_FlagView{ true };
	};
}
