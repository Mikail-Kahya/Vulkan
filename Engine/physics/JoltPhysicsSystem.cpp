//#include "JoltPhysicsSystem.h"
//#include "components/ColliderComponent.h"
//#include "core/Time.h"
//#include "glm/vec3.hpp"
//using namespace mk;
//
//// https://github.com/jrouwe/JoltPhysics/blob/master/HelloWorld/HelloWorld.cpp
//// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
//// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
//// SPDX-License-Identifier: MIT
//#include "Jolt/Jolt.h"
//
//#include <Jolt/RegisterTypes.h>
//#include <Jolt/Core/Factory.h>
//#include <Jolt/Core/TempAllocator.h>
//#include <Jolt/Core/JobSystemThreadPool.h>
//#include <Jolt/Physics/PhysicsSettings.h>
//#include <Jolt/Physics/PhysicsSystem.h>
//#include <Jolt/Physics/Collision/Shape/BoxShape.h>
//#include <Jolt/Physics/Collision/Shape/SphereShape.h>
//#include <Jolt/Physics/Body/BodyCreationSettings.h>
//#include <Jolt/Physics/Body/BodyActivationListener.h>
//
//// STL includes
//#include <iostream>
//#include <thread>
//
//// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
//JPH_SUPPRESS_WARNINGS
//
////========================================================
////==================== JOLT SETUP ========================
////========================================================
//namespace Layers
//{
//static constexpr JPH::ObjectLayer NON_MOVING{ 0 };
//static constexpr JPH::ObjectLayer MOVING{ 1 };
//static constexpr JPH::ObjectLayer NUM_LAYERS{ 2 };
//};
//
//namespace BroadPhaseLayers
//{
//static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
//static constexpr JPH::BroadPhaseLayer MOVING(1);
//static constexpr uint32_t NUM_LAYERS(2);
//};
//
//#ifdef JPH_ENABLE_ASSERTS
//
//// Callback for asserts, connect this to your own assert handler if you have one
//static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, uint32_t inLine)
//{
//    // Print to the TTY
//    std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr? inMessage : "") << std::endl;
//
//    // Breakpoint
//    return true;
//};
//
//#endif // JPH_ENABLE_ASSERTS
//
///// Class that determines if two object layers can collide
//class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
//{
//public:
//    virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
//    {
//        switch (inObject1)
//        {
//        case Layers::NON_MOVING:
//            return inObject2 == Layers::MOVING; // Non moving only collides with moving
//        case Layers::MOVING:
//            return true; // Moving collides with everything
//        default:
//            JPH_ASSERT(false);
//            return false;
//        }
//    }
//};
//
///// Class that determines if an object layer can collide with a broadphase layer
//class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
//{
//public:
//    virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
//    {
//        switch (inLayer1)
//        {
//        case Layers::NON_MOVING:
//            return inLayer2 == BroadPhaseLayers::MOVING;
//        case Layers::MOVING:
//            return true;
//        default:
//            JPH_ASSERT(false);
//            return false;
//        }
//    }
//};
//
//// BroadPhaseLayerInterface implementation
//// This defines a mapping between object and broadphase layers.
//class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
//{
//public:
//    BPLayerInterfaceImpl()
//    {
//        // Create a mapping table from object to broad phase layer
//        m_ObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
//        m_ObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
//    }
//
//    virtual uint32_t GetNumBroadPhaseLayers() const override
//    {
//        return BroadPhaseLayers::NUM_LAYERS;
//    }
//
//    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
//    {
//        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
//        return m_ObjectToBroadPhase[inLayer];
//    }
//
//#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
//    virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
//    {
//        switch ((JPH::BroadPhaseLayer::Type)inLayer)
//        {
//        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
//        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
//        default:													JPH_ASSERT(false); return "INVALID";
//        }
//    }
//#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
//
//private:
//    JPH::BroadPhaseLayer m_ObjectToBroadPhase[Layers::NUM_LAYERS];
//};
//
//// An example contact listener
//class MyContactListener : public JPH::ContactListener
//{
//public:
//    // See: ContactListener
//    virtual JPH::ValidateResult	OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override
//    {
//        std::cout << "Contact validate callback" << std::endl;
//
//        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
//        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
//    }
//
//    virtual void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
//    {
//        std::cout << "A contact was added" << std::endl;
//    }
//
//    virtual void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
//    {
//        std::cout << "A contact was persisted" << std::endl;
//    }
//
//    virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override
//    {
//        std::cout << "A contact was removed" << std::endl;
//    }
//};
//
//// An example activation listener
//class MyBodyActivationListener : public JPH::BodyActivationListener
//{
//public:
//    virtual void OnBodyActivated(const JPH::BodyID &inBodyID, uint64_t inBodyUserData) override
//    {
//        std::cout << "A body got activated" << std::endl;
//    }
//
//    virtual void OnBodyDeactivated(const JPH::BodyID &inBodyID, uint64_t inBodyUserData) override
//    {
//        std::cout << "A body went to sleep" << std::endl;
//    }
//};
//
//class JoltPhysicsSystem::JoltPhysicsImpl final
//{
//public:
//    JoltPhysicsImpl(uint32_t nrMaxBodies);
//    ~JoltPhysicsImpl();
//
//    void Update();
//    collider_id AddBox(const glm::vec3& position, const glm::vec3& boxExtent, bool isStatic);
//
//private:
//    static JPH::Vec3 GlmToJph(const glm::vec3& vec);
//
//    inline static constexpr uint32_t s_NumBodyMutexes = 0;
//    inline static constexpr uint32_t s_MaxBodyPairs = 1024;
//    inline static constexpr uint32_t s_MaxContactConstraints = 1024;
//
//    std::unique_ptr<JPH::Factory> m_Factory{};
//    JPH::PhysicsSystem m_System{};
//    JPH::JobSystemThreadPool m_JobSystem{};
//    JPH::TempAllocatorImpl m_TempAllocator{ 0 };
//
//    MyBodyActivationListener m_BodyActivationListener{};
//    MyContactListener m_ContactListener{};
//};
//
//
//JoltPhysicsSystem::JoltPhysicsSystem(int32_t maxBodies)
//    : m_Physics{ std::make_unique<JoltPhysicsImpl>(maxBodies) }
//{
//}
//
//void JoltPhysicsSystem::Update()
//{
//    m_Physics->Update();
//}
//
//collider_id JoltPhysicsSystem::RegisterCollider(ColliderComponent *colliderPtr)
//{
//    return 0;
//}
//
//void JoltPhysicsSystem::UnregisterCollider(collider_id id)
//{
//
//}
//
////==============================================
////================= JOLT =======================
////==============================================
//
//JoltPhysicsSystem::JoltPhysicsImpl::JoltPhysicsImpl(uint32_t nrMaxBodies)
//    : m_Factory { std::make_unique<JPH::Factory>() }
//    , m_JobSystem(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1)
//    , m_TempAllocator(10 * nrMaxBodies * s_MaxBodyPairs)
//{
//    // This needs to be done before any other Jolt function is called.
//    JPH::RegisterDefaultAllocator();
//    // Install trace and assert callbacks
//    JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)
//    // It is not directly used in this example but still required.
//    JPH::Factory::sInstance = m_Factory.get();
//    // Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
//    JPH::RegisterTypes();
//
//    BPLayerInterfaceImpl broad_phase_layer_interface{};
//    ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter{};
//    ObjectLayerPairFilterImpl object_vs_object_layer_filter{};
//
//    // Now we can create the actual physics system.
//    m_System.Init(nrMaxBodies, s_NumBodyMutexes, s_MaxBodyPairs, s_MaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);
//
//    m_System.SetBodyActivationListener(&m_BodyActivationListener);
//    m_System.SetContactListener(&m_ContactListener);
//
//    m_System.OptimizeBroadPhase();
//}
//
//JoltPhysicsSystem::JoltPhysicsImpl::~JoltPhysicsImpl()
//{
//    JPH::BodyIDVector bodies{};
//    m_System.GetBodies(bodies);
//    m_System.GetBodyInterface().DestroyBodies(bodies.data(), bodies.size());
//    JPH::UnregisterTypes();
//}
//
//void JoltPhysicsSystem::JoltPhysicsImpl::Update()
//{
//    constexpr int nrSteps{ 1 };
//    m_System.Update(Time::DeltaTime(), nrSteps, &m_TempAllocator, &m_JobSystem);
//}
//
//collider_id JoltPhysicsSystem::JoltPhysicsImpl::AddBox(const glm::vec3 &position, const glm::vec3 &boxExtent, bool isStatic)
//{
//    JPH::BodyInterface& bodyInterface{ m_System.GetBodyInterface() };
//
//    // Box setup
//    JPH::BoxShapeSettings boxShapeSettings{ GlmToJph(boxExtent) };
//    boxShapeSettings.SetEmbedded();
//
//    // Create the shape
//    JPH::ShapeSettings::ShapeResult boxShapeResult{ boxShapeSettings.Create() };
//    JPH::ShapeRefC boxShape{ boxShapeResult.Get() };
//
//    // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
//    JPH::BodyCreationSettings boxSettings(boxShape, GlmToJph(position), JPH::Quat::sIdentity(),
//                                          isStatic ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic,
//                                          isStatic ? Layers::NON_MOVING : Layers::MOVING);
//
//    // Create the actual rigid body
//    JPH::Body* boxPtr{ bodyInterface.CreateBody(boxSettings) };
//    if (boxPtr == nullptr)
//        throw std::runtime_error("Not enough space for rigid bodies. Inrease max body pool for physics system");
//    // Add it to the world
//    bodyInterface.AddBody(boxPtr->GetID(), isStatic ? JPH::EActivation::DontActivate : JPH::EActivation::Activate);
//    return 0;
//    //// Now create a dynamic body to bounce on the floor
//    //// Note that this uses the shorthand version of creating and adding a body to the world
//    //BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0_r, 2.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
//    //BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, EActivation::Activate);
//
//    //// Now you can interact with the dynamic body, in this case we're going to give it a velocity.
//    //// (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
//    //body_interface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));
//}
//
//JPH::Vec3 JoltPhysicsSystem::JoltPhysicsImpl::GlmToJph(const glm::vec3 &vec)
//{
//    return { vec.x, vec.y, vec.z };
//}
