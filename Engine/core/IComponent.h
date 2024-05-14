#pragma once
namespace mk
{
	class GameObject;

	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		IComponent(const IComponent& other) = delete;
		IComponent(IComponent&& other) = delete;
		IComponent& operator=(const IComponent& other) = delete;
		IComponent& operator=(IComponent&& other) = delete;

		virtual void Start() {}

		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}

		void Destroy();
		void ClearDestroy();

		bool DestroyFlagged() const;
		GameObject* GetOwner() const;

	protected:
		IComponent() = default;

	private:
		friend class GameObject;
		void SetOwner(GameObject* ownerPtr);

		GameObject* m_OwnerPtr{};
		bool m_Destroy{};
	};
}


