#include "SpriteComponent.h"

#include "core/GameObject.h"
#include "core/ServiceLocator.h"
#include "core/ResourceManager.h"

using namespace mk;

SpriteComponent::~SpriteComponent()
{
	ServiceLocator::GetRenderer().UnregisterSpriteRender(m_Handle);
}

void SpriteComponent::SetShader(const std::string& vertex, const std::string& fragment)
{
	ServiceLocator::GetRenderer().UnregisterSpriteRender(m_Handle);
	m_Handle = -1;
	m_Pipeline = ResourceManager::LoadShader2D(vertex, fragment);
	ServiceLocator::GetRenderer().RegisterSpriteRender(this);
}

void SpriteComponent::SetMesh(const std::string& obj)
{
	ServiceLocator::GetRenderer().UnregisterSpriteRender(m_Handle);
	m_Handle = -1;
	m_MeshPtr = ResourceManager::LoadMesh2D(obj);
	ServiceLocator::GetRenderer().RegisterSpriteRender(this);
}

Mesh2D* SpriteComponent::GetMesh() const
{
	return m_MeshPtr;
}

Pipeline2D* SpriteComponent::GetPipeline() const
{
	return m_Pipeline;
}