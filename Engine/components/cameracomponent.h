#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "core/IComponent.h"

namespace mk
{
    class CameraComponent final : public IComponent
    {
    public:
        CameraComponent();
        ~CameraComponent() override;

        CameraComponent(const CameraComponent& other)				= delete;
        CameraComponent(CameraComponent&& other)					= delete;
        CameraComponent& operator=(const CameraComponent& other)	= delete;
        CameraComponent& operator=(CameraComponent&& other)			= delete;

        void Activate();
    };
}

#endif // CAMERACOMPONENT_H
