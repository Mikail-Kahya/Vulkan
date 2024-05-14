#include "MeshComponent.h"

#include <windows.h>

#include "core/GameObject.h"
#include "core/ServiceLocator.h"
#include "core/ResourceManager.h"

using namespace mk;

MeshComponent::~MeshComponent()
{
	ServiceLocator::GetRenderer().UnregisterRender(this);
}

void MeshComponent::Start()
{
	ServiceLocator::GetRenderer().RegisterRender(this);
}