#include "IComponent.h"

using namespace mk;

void IComponent::SetOwner(GameObject* ownerPtr)
{
	m_OwnerPtr = ownerPtr;
}

void IComponent::Destroy()
{
	m_Destroy = true;
}

void IComponent::ClearDestroy()
{
	m_Destroy = false;
}

bool IComponent::DestroyFlagged() const
{
	return m_Destroy;
}

GameObject* IComponent::GetOwner() const
{
	return m_OwnerPtr;
}
