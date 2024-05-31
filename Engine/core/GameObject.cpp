#include <string>
#include <algorithm>

#include "GameObject.h"
#include "IComponent.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

using namespace mk;

GameObject::GameObject(const std::string& name)
	: m_Name{ name }
	, m_Components{}
	, m_ComponentBuffer{}
{
}

GameObject::~GameObject() = default;

GameObject::GameObject(GameObject&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_Destroy{ other.m_Destroy }
	, m_LocalTransform{ other.m_LocalTransform }
	, m_WorldTransform{ other.m_WorldTransform }
	, m_Parent{ other.m_Parent }
	, m_Children{ std::move(other.m_Children) }
	, m_Components{ std::move(other.m_Components) }
	, m_ComponentBuffer{ std::move(other.m_ComponentBuffer) }
	
{
	other.m_Parent = nullptr;
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_Destroy = other.m_Destroy;
	m_LocalTransform = std::move(other.m_LocalTransform);
	m_WorldTransform = std::move(other.m_WorldTransform);
	m_Parent = other.m_Parent;
	m_Children = std::move(other.m_Children);
	m_Components = std::move(other.m_Components);
	m_ComponentBuffer = std::move(other.m_ComponentBuffer);

	other.m_Parent = nullptr;

	return *this;
}

void GameObject::RemoveComponent(const std::unique_ptr<IComponent>& component)
{
	component->Destroy();
}

void GameObject::Update()
{
	for (const auto& component : m_Components)
		component->Update();
}

void GameObject::FixedUpdate()
{
	for (const auto& component : m_Components)
		component->FixedUpdate();
}

void GameObject::LateUpdate()
{
	for (const auto& component : m_Components)
		component->LateUpdate();

	ComponentCleanup();
}

void GameObject::ComponentCleanup()
{
	// Remove destroy flagged components
	auto eraseIt = std::stable_partition(m_Components.begin(), m_Components.end(), [](const std::unique_ptr<IComponent>& component)
		{
			return !component->DestroyFlagged();
		});

	m_Components.erase(eraseIt, m_Components.end());

	// Move components and flush buffer
	m_Components.insert(m_Components.end(),
		std::make_move_iterator(m_ComponentBuffer.begin()),
		std::make_move_iterator(m_ComponentBuffer.end()));
	m_ComponentBuffer.clear();
}

void GameObject::AddChild(GameObject* childPtr)
{
	m_Children.push_back(childPtr);
}

void GameObject::RemoveChild(GameObject* childPtr)
{
	m_Children.erase(std::ranges::find(m_Children, childPtr));
}

bool GameObject::IsChild(GameObject* childPtr) const
{
	const auto foundChild{ std::ranges::find(m_Children, childPtr) };
	return foundChild != m_Children.cend();
}

void GameObject::Destroy()
{
	m_Destroy = true;
	for (GameObject* childPtr : m_Children)
		childPtr->Destroy();
}

void GameObject::ClearDestroy()
{
	m_Destroy = false;
	for (GameObject* childPtr : m_Children)
		childPtr->ClearDestroy();
}

bool GameObject::DestroyFlagged() const
{
	return m_Destroy;
}

const std::string& GameObject::GetName() const
{
	return m_Name;
}

bool GameObject::IsStatic() const
{
	return m_IsStatic;
}

const glm::vec3& GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
        UpdateWorldPosition();
	return m_WorldTransform.GetPosition();
}

const glm::vec3& GameObject::GetWorldRotation()
{
	if (m_RotationIsDirty)
        UpdateWorldRotation();
	return m_WorldTransform.GetRotation();
}

const glm::vec3& GameObject::GetWorldScale()
{
	if (m_ScaleIsDirty)
        UpdateWorldScale();
	return m_WorldTransform.GetScale();
}

const glm::vec3& GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}

const glm::vec3& GameObject::GetLocalRotation() const
{
	return m_LocalTransform.GetRotation();
}

const glm::vec3& GameObject::GetLocalScale() const
{
	return m_LocalTransform.GetScale();
}

const glm::vec3& GameObject::GetForward()
{
	if (m_RotationIsDirty)
		UpdateWorldRotation();
	return m_Forward;
}

const glm::vec3& GameObject::GetUp()
{
	if (m_RotationIsDirty)
		UpdateWorldRotation();
	return m_Up;
}

const glm::vec3& GameObject::GetRight()
{
	if (m_RotationIsDirty)
		UpdateWorldRotation();
	return m_Right;
}

void GameObject::SetLocalPosition(const glm::vec3& position)
{
	m_LocalTransform.SetPosition(position);
	FlagPositionDirty();
}

void GameObject::SetLocalRotation(const glm::vec3& rotation)
{
	m_LocalTransform.SetRotation(rotation);
	FlagRotationDirty();
}

void GameObject::SetLocalScale(const glm::vec3& scale)
{
	m_LocalTransform.SetScale(scale);
	FlagScaleDirty();
}

void GameObject::AddLocalOffset(const glm::vec3& offset)
{
	m_LocalTransform.AddOffset(offset);
	FlagPositionDirty();
}

void GameObject::AddLocalRotation(const glm::vec3& deltaRotation)
{
	m_LocalTransform.AddRotation(deltaRotation);
	FlagRotationDirty();
}

void GameObject::AddLocalScale(const glm::vec3& deltaScale)
{
	m_LocalTransform.AddScale(deltaScale);
	FlagScaleDirty();
}

void GameObject::AddPitch(float deltaPitch)
{
	AddLocalRotation({ deltaPitch, 0, 0 });
    FlagRotationDirty();
}

void GameObject::AddYaw(float deltaYaw)
{
	AddLocalRotation({ 0, deltaYaw, 0 });
    FlagRotationDirty();
}

void GameObject::AddRoll(float deltaRoll)
{
	AddLocalRotation({ 0, 0, deltaRoll });
    FlagRotationDirty();
}

void GameObject::SetStatic(bool isStatic)
{
	m_IsStatic = isStatic;
}

void GameObject::UpdateWorldPosition()
{
	if (!m_PositionIsDirty)
		return;

	if (m_Parent == nullptr)
		m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
	else
    {
        glm::vec3 pos{ m_Parent->GetForward() * m_LocalTransform.GetPosition().z * m_LocalTransform.GetScale().z };
        pos += m_Parent->GetRight() * m_LocalTransform.GetPosition().x * m_LocalTransform.GetScale().x;
        pos += m_Parent->GetUp() * m_LocalTransform.GetPosition().y * m_LocalTransform.GetScale().y;
		m_WorldTransform.SetPosition(GetParent()->GetWorldPosition() + pos);
    }

	m_PositionIsDirty = false;
}

void GameObject::UpdateWorldRotation()
{
	if (!m_RotationIsDirty)
		return;

	if (m_Parent == nullptr)
		m_WorldTransform.SetRotation(m_LocalTransform.GetRotation());
	else
	{
		m_WorldTransform.SetRotation(m_Parent->GetWorldRotation() + m_LocalTransform.GetRotation());
		FlagPositionDirty();
	}

	UpdateAxis();
	m_RotationIsDirty = false;
}

void GameObject::UpdateWorldScale()
{
	if (!m_ScaleIsDirty)
		return;

	if (m_Parent == nullptr)
		m_WorldTransform.SetScale(m_LocalTransform.GetScale());
	else
		m_WorldTransform.SetScale(m_Parent->GetWorldScale() * m_LocalTransform.GetScale());

	m_ScaleIsDirty = false;
}

void GameObject::UpdateAxis()
{
	const glm::vec3& rotation{ m_WorldTransform.GetRotation() };
	glm::mat4 rotator = glm::rotate(glm::mat4{ 1 }, glm::radians(rotation.y), glm::vec3{ 0, 1, 0 });
	rotator = glm::rotate(rotator, glm::radians(rotation.x), glm::vec3{ 1, 0, 0 });
	rotator = glm::rotate(rotator, glm::radians(rotation.z), glm::vec3{ 0, 0, 1 });

	m_Forward = glm::normalize(rotator * glm::vec4{ 0, 0, 1, 1 });
    m_Right = glm::normalize(glm::cross(m_Forward, { 0, 1, 0 }));
    m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}

void GameObject::FlagPositionDirty()
{
	m_PositionIsDirty = true;
	for (GameObject* childPtr : m_Children)
		childPtr->FlagPositionDirty();
}

void GameObject::FlagRotationDirty()
{
	m_RotationIsDirty = true;
	for (GameObject* childPtr : m_Children)
		childPtr->FlagRotationDirty();
}

void GameObject::FlagScaleDirty()
{
	m_ScaleIsDirty = true;
	for (GameObject* childPtr : m_Children)
		childPtr->FlagScaleDirty();
}

void GameObject::SetParent(GameObject* parentPtr, bool keepWorldTransform)
{
	if (m_Parent == parentPtr || this == parentPtr || IsChild(parentPtr))
		return;

	if (parentPtr == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
		SetLocalRotation(GetWorldRotation());
		SetLocalScale(GetWorldScale());
	}
	else
	{
		if (keepWorldTransform)
		{
			SetLocalPosition(m_WorldTransform.GetPosition() - parentPtr->GetWorldPosition());
			SetLocalRotation(m_WorldTransform.GetRotation() - parentPtr->GetWorldRotation());
			SetLocalScale(m_WorldTransform.GetScale() - parentPtr->GetWorldScale());
		}
		FlagPositionDirty();
		FlagRotationDirty();
		FlagScaleDirty();
	}

	if (m_Parent != nullptr)
		m_Parent->RemoveChild(this);
	m_Parent = parentPtr;
	if (m_Parent != nullptr)
		m_Parent->AddChild(this);
}

GameObject* GameObject::GetParent() const
{
	return m_Parent;
}

int GameObject::GetChildCount() const
{
	return static_cast<int>(m_Children.size());
}

GameObject* GameObject::GetChildAt(int index) const
{
	if (index > GetChildCount())
		return nullptr;
	return m_Children[index];
}

GameObject* GameObject::GetChildWithName(const std::string& name, bool recursively) const
{
	GameObject* foundChildPtr = *std::find_if(m_Children.begin(), m_Children.end(), [&name](GameObject* childPtr)
		{
			return childPtr->GetName() == name;
		});

	if (foundChildPtr != nullptr)
		return foundChildPtr;

	if (!recursively)
		return nullptr;

	for (const GameObject* childPtr : m_Children)
	{
		foundChildPtr = childPtr->GetChildWithName(name, recursively);
		if (foundChildPtr != nullptr)
			return foundChildPtr;
	}


	return nullptr;
}
