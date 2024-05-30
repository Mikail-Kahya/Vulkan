#include "cameracomponent.h"
#include "core/ServiceLocator.h"
using namespace mk;

CameraComponent::CameraComponent()
{
    ServiceLocator::GetRenderer().RegisterCamera(this);
}

CameraComponent::~CameraComponent()
{
    ServiceLocator::GetRenderer().UnregisterCamera(this);
}

void CameraComponent::Activate()
{
    ServiceLocator::GetRenderer().SetActiveCamera(this);
}
