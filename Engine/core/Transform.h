#pragma once
#include <glm/vec3.hpp>

namespace mk
{
	class Transform final
	{
	public:
		Transform() = default;

		const glm::vec3& GetPosition() const;
		const glm::vec3& GetRotation() const;
		const glm::vec3& GetScale() const;

		void AddOffset(const glm::vec3& offset);
		void AddRotation(const glm::vec3& deltaRotation);
		void AddScale(const glm::vec3& deltaScale);
		void AddPitch(float deltaPitch);
		void AddYaw(float deltaYaw);
		void AddRoll(float deltaRoll);
		

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);

	private:
		static float OverflowAngle(float angle);

		glm::vec3 m_Position{};
		glm::vec3 m_Rotation{};
		glm::vec3 m_Scale{ 1.f, 1.f, 1.f };
	};
}
