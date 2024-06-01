#ifndef SAUCERSTATES_H
#define SAUCERSTATES_H

#include "state/GameObjectState.h"
#include "glm/vec3.hpp"
namespace mk
{

    class LandingState : public GameObjectState
    {
    public:
        LandingState() = default;
        ~LandingState() override = default;

        LandingState(const LandingState& other)					= delete;
        LandingState(LandingState&& other) noexcept				= delete;
        LandingState& operator=(const LandingState& other)		= delete;
        LandingState& operator=(LandingState&& other) noexcept	= delete;

        void Start() override;

        void Update() override;
        void OnEnter() override;
        std::string TransitionState() override;

    private:
        static constexpr float LANDING_TIME{ 2.f };
        glm::vec3 m_LandingSpot{};
        glm::vec3 m_StartingSpot{};
    	float m_Timer{};
    };

    class FlyingState : public GameObjectState
    {
    public:
        FlyingState() = default;
        ~FlyingState() override = default;

        FlyingState(const FlyingState& other)                   = delete;
        FlyingState(FlyingState&& other) noexcept               = delete;
        FlyingState& operator=(const FlyingState& other)        = delete;
        FlyingState& operator=(FlyingState&& other) noexcept    = delete;

        void OnEnter() override;
        void Update() override;
        std::string TransitionState() override;

    private:
        static constexpr float FLYING_TIME{ 2.f };
        static constexpr float FLYING_SPEED{ 100.f };
        static constexpr float ROTATION_SPEED{ 100.f };
        glm::vec3 m_FlyingDirection{};
        float m_Timer{};
    };
}

#endif // SAUCERSTATES_H
