#include "playercommands.h"
#include "core/GameObject.h"
#include "core/Time.h"

using namespace mk;

MoveCommand::MoveCommand(GameObject *gameObject, const glm::vec3 &direction, float speed)
    : GameObjectCommand(gameObject)
    , m_Direction{ direction }
    , m_Speed{ speed }
{
}

void MoveCommand::Execute()
{
    glm::vec3 moveDir{ GetGameObject().GetForward()};
    moveDir += m_Direction.x * GetGameObject().GetRight();
    moveDir += m_Direction.y * GetGameObject().GetUp();
    moveDir += m_Direction.z * GetGameObject().GetForward();
   
    GetGameObject().AddLocalOffset(m_Speed * Time::DeltaTime() * moveDir);
}

FireCommand::FireCommand(GameObject *gameObject)
    : GameObjectCommand(gameObject)
{
}

void FireCommand::Execute()
{

}

RotateCommand::RotateCommand(GameObject* gameObject, const glm::vec2& rotateSpeed)
    : DirectionCommand(gameObject)
    , m_RotateSpeed{ rotateSpeed }
{
}

void RotateCommand::Execute(const glm::vec2& direction)
{
    const glm::vec2 deltaRot{ m_RotateSpeed * Time::DeltaTime() };
    GetGameObject().AddYaw(direction.x * deltaRot.x);
    GetGameObject().AddPitch(direction.y * deltaRot.y);
    //GetGameObject().AddPitch(rotation.y);
}
