#include "MeshComponent.h"

#include "core/ServiceLocator.h"
#include "core/ResourceManager.h"

using namespace mk;

MeshComponent::~MeshComponent()
{
	ServiceLocator::GetRenderer().UnregisterRender(m_Handle);
}

void MeshComponent::SetShader(const std::string& vertex, const std::string& fragment)
{
	ServiceLocator::GetRenderer().UnregisterRender(m_Handle);
	m_Handle = -1;
	m_Pipeline = ResourceManager::LoadShader3D(vertex, fragment);
	ServiceLocator::GetRenderer().RegisterRender(this);
}

void MeshComponent::SetMesh(const std::string& obj)
{
	ServiceLocator::GetRenderer().UnregisterRender(m_Handle);
	m_Handle = -1;
	m_MeshPtr = ResourceManager::LoadMesh3D(obj);
	ServiceLocator::GetRenderer().RegisterRender(this);
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