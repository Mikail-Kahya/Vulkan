#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Transform.h"

namespace mk
{

	class IComponent;
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject(const std::string& name = {});
		~GameObject();

		// TODO fix
		GameObject(const GameObject& other)					= delete;
		GameObject(GameObject&& other) noexcept;
		GameObject& operator=(const GameObject& other)		= delete;
		GameObject& operator=(GameObject&& other) noexcept;

		void Update();
		void FixedUpdate();
		void LateUpdate();

		void Destroy();
		void ClearDestroy();
		bool DestroyFlagged() const;

		const std::string& GetName() const;
		bool IsStatic() const;

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetWorldRotation();
		const glm::vec3& GetWorldScale();

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetLocalRotation() const;
		const glm::vec3& GetLocalScale() const;
		
		const glm::vec3& GetForward();
		const glm::vec3& GetUp();
		const glm::vec3& GetRight();

		void SetLocalPosition(const glm::vec3& position);
		void SetLocalRotation(const glm::vec3& rotation);
		void SetLocalScale(const glm::vec3& scale);

		void AddLocalOffset(const glm::vec3& offset);
		void AddLocalRotation(const glm::vec3& deltaRotation);
		void AddLocalScale(const glm::vec3& deltaScale);

		void AddPitch(float deltaPitch);
		void AddYaw(float deltaYaw);
		void AddRoll(float deltaRoll);

		void SetStatic(bool isStatic);

		// TODO: Keep world rotation?
		void SetParent(GameObject* parentPtr, bool keepWorldTransform = false);
		GameObject* GetParent() const;
		int GetChildCount() const;
		GameObject* GetChildAt(int index) const;
		GameObject* GetChildWithName(const std::string& name, bool recursively = false) const;

		template <std::derived_from<IComponent> ComponentType>
		[[nodiscard]] ComponentType* GetComponent() const;
		template <std::derived_from<IComponent> ComponentType, typename... Args>
		ComponentType* AddComponent(Args... args);
		void RemoveComponent(const std::unique_ptr<IComponent>& component);


	private:
		void ComponentCleanup();

		void UpdateTransform();

		void UpdateWorldPosition();
		void UpdateWorldRotation();
		void UpdateWorldScale();
		void UpdateAxis();

		void FlagPositionDirty();
		void FlagRotationDirty();
		void FlagScaleDirty();

		void AddChild(GameObject* childPtr);
		void RemoveChild(GameObject* childPtr);

		bool IsChild(GameObject* childPtr) const;

		// Ownership
		GameObject* m_Parent{};
		std::vector<GameObject*> m_Children{};

		// Components
		std::vector<std::unique_ptr<IComponent>> m_Components;
		std::vector<std::unique_ptr<IComponent>> m_ComponentBuffer;

		// Common state
		std::string m_Name{};
		Transform m_LocalTransform{};
		Transform m_WorldTransform{};
		glm::vec3 m_Forward{ 0, 0, 1 };
		glm::vec3 m_Right{ 1, 0, 0 };
		glm::vec3 m_Up{ 0,1,0 };
		bool m_PositionIsDirty{ false };
		bool m_RotationIsDirty{ false };
		bool m_ScaleIsDirty{ false };
		bool m_IsStatic{ false };
		bool m_Destroy{ false };
	};

	template <std::derived_from<IComponent> ComponentType>
	ComponentType* GameObject::GetComponent() const
	{
        auto componentIt = std::find_if(m_Components.begin(), m_Components.end(),
            [](const std::unique_ptr<IComponent>& component)
			{
				return dynamic_cast<ComponentType*>(component.get());
			});

		if (componentIt != m_Components.end())
			return dynamic_cast<ComponentType*>(componentIt->get());

        componentIt = std::find_if(m_ComponentBuffer.begin(), m_ComponentBuffer.end(),
            [](const std::unique_ptr<IComponent>& component)
			{
				return dynamic_cast<ComponentType*>(component.get());
			});

		if (componentIt != m_ComponentBuffer.end())
			return dynamic_cast<ComponentType*>(componentIt->get());

		return nullptr;
	}

	template <std::derived_from<IComponent> ComponentType, typename... Args>
	ComponentType* GameObject::AddComponent(Args... args)
	{
		ComponentType* componentPtr{ GetComponent<ComponentType>() };
		if (componentPtr != nullptr)
			return componentPtr;

		std::unique_ptr<ComponentType> component{ std::make_unique<ComponentType>(std::move(args)...)};
		component->SetOwner(this);
		component->Start();

		componentPtr = component.get();
		m_ComponentBuffer.emplace_back(std::move(component));
		return componentPtr;
	}
}
