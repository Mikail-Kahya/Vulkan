#include "MeshComponent.h"

#include "core/GameObject.h"
#include "core/ServiceLocator.h"
#include "core/ResourceManager.h"

using namespace mk;

MeshComponent::~MeshComponent()
{
	ServiceLocator::GetRenderer().UnregisterMeshRender(m_Handle);
}

void MeshComponent::LateUpdate()
{
	IComponent::LateUpdate();
	m_MeshPtr->SetPosition(GetOwner()->GetWorldPosition());
	m_MeshPtr->SetRotation(GetOwner()->GetWorldRotation());
	m_MeshPtr->SetScale(GetOwner()->GetWorldScale());
}

void MeshComponent::SetShader(const std::string& vertex, const std::string& fragment)
{
	ServiceLocator::GetRenderer().UnregisterMeshRender(m_Handle);
	m_Handle = -1;
	m_Pipeline = ResourceManager::LoadShader3D(vertex, fragment);
	ServiceLocator::GetRenderer().RegisterMeshRender(this);
}

void MeshComponent::SetMesh(const std::string& obj)
{
	ServiceLocator::GetRenderer().UnregisterMeshRender(m_Handle);
	m_Handle = -1;
	m_MeshPtr = ResourceManager::LoadMesh3D(obj);
	ServiceLocator::GetRenderer().RegisterMeshRender(this);
}

void MeshComponent::SetTexture(const std::string& texture) const
{
	m_MeshPtr->SetTexture(texture);
}

Mesh3D* MeshComponent::GetMesh() const
{
	return m_MeshPtr;
}

Pipeline3D* MeshComponent::GetPipeline() const
{
	return m_Pipeline;
}