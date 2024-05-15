#include <stdexcept>
#include <cstring>

#include "VulkanRenderer.h"

#include "tiny_obj_loader.h"
#include "vulkan/VulkanBase.h"
#include "abstraction/Mesh2D.h"
#include "abstraction/Mesh3D.h"
#include "abstraction/Pipeline2D.h"
#include "abstraction/Pipeline3D.h"
#include "components/MeshComponent.h"
#include "core/ResourceManager.h"
#include "core/Utils.h"

using namespace mk;

class VulkanRenderer::VulkanImpl final
{
	using Meshes3D = std::vector<Mesh3D*>;
	using Meshes2D = std::vector<Mesh2D*>;

public:
	VulkanImpl() { VulkanBase::GetInstance().InitVulkan(); }
	~VulkanImpl()
	{
		ResourceManager::Cleanup();
		VulkanBase::GetInstance().Cleanup();
	}

	void Render() const;

	void AddMesh2D(Pipeline2D* pipelinePtr, Mesh2D* meshPtr);
	void AddMesh3D(Pipeline3D* pipelinePtr, Mesh3D* meshPtr);
	void RemoveMesh2D(Pipeline2D* pipelinePtr, Mesh2D* meshPtr);
	void RemoveMesh3D(Pipeline3D* pipelinePtr, Mesh3D* meshPtr);

private:
	std::unordered_map<Pipeline2D*, Meshes2D> m_Mesh2DSets{};
	std::unordered_map<Pipeline3D*, Meshes3D> m_Mesh3DSets{};
};

void VulkanRenderer::VulkanImpl::Render() const
{
	for (const auto& meshSet : m_Mesh3DSets)
	{
		meshSet.first->StartDrawing();
		for (const auto& mesh : meshSet.second)
		{
			mesh->Update();
			meshSet.first->Draw(mesh);
		}
		meshSet.first->EndDrawing();
	}

	for (const auto& meshSet : m_Mesh2DSets)
	{
		meshSet.first->StartDrawing();
		for (const auto& mesh : meshSet.second)
			meshSet.first->Draw(mesh);
		meshSet.first->EndDrawing();
	}
}

void VulkanRenderer::VulkanImpl::AddMesh2D(Pipeline2D* pipelinePtr, Mesh2D* meshPtr)
{
	m_Mesh2DSets[pipelinePtr].emplace_back(meshPtr);
}

void VulkanRenderer::VulkanImpl::AddMesh3D(Pipeline3D* pipelinePtr, Mesh3D* meshPtr)
{
	m_Mesh3DSets[pipelinePtr].emplace_back(meshPtr);
}

void VulkanRenderer::VulkanImpl::RemoveMesh2D(Pipeline2D* pipelinePtr, Mesh2D* meshPtr)
{
	 std::erase_if(m_Mesh2DSets[pipelinePtr], [meshPtr](auto meshCheckPtr)
	 {
			 return meshPtr == meshCheckPtr;
	 });
}

void VulkanRenderer::VulkanImpl::RemoveMesh3D(Pipeline3D* pipelinePtr, Mesh3D* meshPtr)
{
	std::erase_if(m_Mesh3DSets[pipelinePtr], [meshPtr](auto meshCheckPtr)
		{
			return meshPtr == meshCheckPtr;
		});
}

VulkanRenderer::VulkanRenderer(int width, int height)
	: m_Impl{ std::make_unique<VulkanImpl>() }
{
}


void VulkanRenderer::Render() const
{
	m_Impl->Render();
}

int VulkanRenderer::GetHeight() const noexcept
{
	return 0;
}

int VulkanRenderer::GetWidth() const noexcept
{
	return 0;
}

mesh_handle VulkanRenderer::RegisterRender(void* renderPtr)
{
	MeshComponent* meshCompPtr = reinterpret_cast<MeshComponent*>(renderPtr);
	m_Impl->AddMesh3D(meshCompPtr->GetPipeline(), meshCompPtr->GetMesh());
	
	const auto invalidSlot = std::find(m_Renderers.begin(), m_Renderers.end(), nullptr);
	if (invalidSlot == m_Renderers.end())
	{
		m_Renderers.emplace_back(meshCompPtr);
		return m_Renderers.size() - 1;
	}

	return static_cast<mesh_handle>(std::distance(m_Renderers.begin(), invalidSlot));
}

void VulkanRenderer::UnregisterRender(mesh_handle handle)
{
	m_Renderers[handle] = nullptr;
}
