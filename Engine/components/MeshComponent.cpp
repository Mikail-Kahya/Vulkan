#include "MeshComponent.h"

#include "core/ServiceLocator.h"
#include "core/ResourceManager.h"

using namespace mk;

MeshComponent::MeshComponent(std::string vertex, std::string fragment, bool projectedOnScreen)
	: m_VertexShader{ std::move(vertex) }
	, m_FragmentShader{ std::move(fragment) }
	, m_ProjectedOnScreen{ projectedOnScreen }
{
}

MeshComponent::~MeshComponent()
{
	ServiceLocator::GetRenderer().UnregisterRender(this);
}

void MeshComponent::Start()
{
	ServiceLocator::GetRenderer().RegisterRender(this);
}

void MeshComponent::SetTexture(const std::string& texture)
{
}

const std::string& MeshComponent::GetVertexShader() const
{
	return m_VertexShader;
}

const std::string& MeshComponent::GetFragmentShader() const
{
	return m_FragmentShader;
}

bool MeshComponent::IsProjected() const
{
	return m_ProjectedOnScreen;
}
