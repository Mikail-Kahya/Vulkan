#ifndef PLAYERCOMMANDS_H
#define PLAYERCOMMANDS_H

#include "input/Command.h"
#include "glm/vec3.hpp"
namespace mk
{
class MovementComponent;

    class MoveCommand : public GameObjectCommand
    {
    public:
        MoveCommand(GameObject* gameObject, const glm::vec3& direction, float speed);
        ~MoveCommand() override = default;

        MoveCommand(const MoveCommand& other)					= delete;
        MoveCommand(MoveCommand&& other) noexcept				= delete;
        MoveCommand& operator=(const MoveCommand& other)		= delete;
        MoveCommand& operator=(MoveCommand&& other) noexcept	= delete;

        void Execute() override;

    private:
        const glm::vec3 m_Direction{};
        const float m_Speed{};
    };


    class FireCommand : public GameObjectCommand
    {
    public:
        FireCommand(GameObject* gameObject);
        ~FireCommand() override = default;

        FireCommand(const FireCommand& other) = delete;
        FireCommand(FireCommand&& other) noexcept = delete;
        FireCommand& operator=(const FireCommand& other) = delete;
        FireCommand& operator=(FireCommand&& other) noexcept = delete;

        void Execute() override;

    private:
        //FireComponent* m_FireCompPtr{};
    };

    class RotateCommand : public DirectionCommand
    {
    public:
        RotateCommand(GameObject* gameObject, const  glm::vec2& rotateSpeed);
        ~RotateCommand() override = default;

        RotateCommand(const RotateCommand& other) = delete;
        RotateCommand(RotateCommand&& other) noexcept = delete;
        RotateCommand& operator=(const RotateCommand& other) = delete;
        RotateCommand& operator=(RotateCommand&& other) noexcept = delete;

		void Execute(const glm::vec2& direction) override;

    private:
        const glm::vec2 m_RotateSpeed{};
    };
}

#endif // PLAYERCOMMANDS_H
