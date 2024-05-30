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
    GetGameObject().AddLocalOffset(m_Speed * Time::DeltaTime() * m_Direction);
}

FireCommand::FireCommand(GameObject *gameObject)
    : GameObjectCommand(gameObject)
{
}

void FireCommand::Execute()
{

}

RotateCommand::RotateCommand(GameObject *gameObject, float rotateSpeed)
    : DirectionCommand(gameObject)
    , m_RotateSpeed{ rotateSpeed }
{
}

void RotateCommand::Execute(const glm::vec2& direction)
{
    const glm::vec2 rotation{ direction * m_RotateSpeed * Time::DeltaTime() };
    GetGameObject().AddYaw(rotation.x);
    //GetGameObject().AddPitch(rotation.y);
}
