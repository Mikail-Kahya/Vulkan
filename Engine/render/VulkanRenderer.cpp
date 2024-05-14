#include <stdexcept>
#include <cstring>

#include "VulkanRenderer.h"
#include "vulkan/VulkanBase.h"

using namespace mk;

VulkanRenderer::VulkanRenderer(int width, int height)
{
}

void VulkanRenderer::Render() const
{
}

int VulkanRenderer::GetHeight() const noexcept
{
	return 0;
}

int VulkanRenderer::GetWidth() const noexcept
{
	return 0;
}

void VulkanRenderer::RegisterRender(void* renderPtr)
{
}

void VulkanRenderer::UnregisterRender(void* renderPtr)
{
}
