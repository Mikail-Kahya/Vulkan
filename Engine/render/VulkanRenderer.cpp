#include "VulkanRenderer.h"

#include "tiny_obj_loader.h"
#include "vulkan/VulkanBase.h"
#include "abstraction/Mesh2D.h"
#include "abstraction/Mesh3D.h"
#include "abstraction/Pipeline2D.h"
#include "abstraction/Pipeline3D.h"
#include "components/MeshComponent.h"
#include "components/cameracomponent.h"
#include "components/SpriteComponent.h"
#include "core/GameObject.h"

using namespace mk;

class VulkanRenderer::VulkanImpl final
{
	using Meshes3D = std::vector<Mesh3D*>;
	using Meshes2D = std::vector<Mesh2D*>;

public:
	VulkanImpl() = default;
	~VulkanImpl() = default;

	VulkanImpl(const VulkanImpl& other)					= delete;
	VulkanImpl(VulkanImpl&& other) noexcept				= delete;
	VulkanImpl& operator=(const VulkanImpl& other)		= delete;
	VulkanImpl& operator=(VulkanImpl&& other) noexcept	= delete;

    void Render(CameraComponent* cameraPtr) const;
	int GetHeight() const;
	int GetWidth() const;

	void AddMesh2D(Pipeline2D* pipelinePtr, Mesh2D* meshPtr);
	void AddMesh3D(Pipeline3D* pipelinePtr, Mesh3D* meshPtr);
	void RemoveMesh2D(Pipeline2D* pipelinePtr, Mesh2D* meshPtr);
	void RemoveMesh3D(Pipeline3D* pipelinePtr, Mesh3D* meshPtr);

private:
	std::unordered_map<Pipeline2D*, Meshes2D> m_Mesh2DSets{};
	std::unordered_map<Pipeline3D*, Meshes3D> m_Mesh3DSets{};
};

void VulkanRenderer::VulkanImpl::Render(CameraComponent* cameraPtr) const
{
    Camera& camera{  VulkanBase::GetInstance().GetCamera() };
    camera.SetPosition(cameraPtr->GetOwner()->GetWorldPosition());
    camera.SetRotation(cameraPtr->GetOwner()->GetWorldRotation());
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

int VulkanRenderer::VulkanImpl::GetHeight() const
{
	return VulkanBase::GetInstance().GetSwapChain().GetHeight();
}

int VulkanRenderer::VulkanImpl::GetWidth() const
{
	return VulkanBase::GetInstance().GetSwapChain().GetWidth();
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

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::Render() const
{
    if (!m_RenderCamera)
        return;

    VulkanBase::GetInstance().DrawFrame([this]()
                                        {
                                            m_Impl->Render(m_RenderCamera);
                                        });

}

void VulkanRenderer::Update()
{
	for (auto renderComp : m_Meshes)
        renderComp->LateUpdate();
    m_RenderCamera->GetOwner()->GetWorldRotation();
    m_RenderCamera->GetOwner()->GetWorldPosition();
    m_RenderCamera->GetOwner()->GetWorldScale();
}

int VulkanRenderer::GetHeight() const noexcept
{
	return m_Impl->GetHeight();
}

int VulkanRenderer::GetWidth() const noexcept
{
	return m_Impl->GetWidth();
}

mesh_handle VulkanRenderer::RegisterMeshRender(MeshComponent* meshCompPtr)
{
	if (meshCompPtr->GetPipeline() == nullptr || meshCompPtr->GetMesh() == nullptr)
		return -1;

	m_Impl->AddMesh3D(meshCompPtr->GetPipeline(), meshCompPtr->GetMesh());
	
	const auto invalidSlot = std::find(m_Meshes.begin(), m_Meshes.end(), nullptr);
	if (invalidSlot == m_Meshes.end())
	{
		m_Meshes.emplace_back(meshCompPtr);
		return static_cast<mesh_handle>(m_Meshes.size() - 1);
	}

	*invalidSlot = meshCompPtr;

	return static_cast<mesh_handle>(std::distance(m_Meshes.begin(), invalidSlot));
}

mesh_handle VulkanRenderer::RegisterSpriteRender(SpriteComponent* objectPtr)
{
	if (objectPtr->GetPipeline() == nullptr || objectPtr->GetMesh() == nullptr)
		return -1;

	m_Impl->AddMesh2D(objectPtr->GetPipeline(), objectPtr->GetMesh());

	const auto invalidSlot = std::find(m_Sprites.begin(), m_Sprites.end(), nullptr);
	if (invalidSlot == m_Sprites.end())
	{
		m_Sprites.emplace_back(objectPtr);
		return static_cast<mesh_handle>(m_Sprites.size() - 1);
	}

	*invalidSlot = objectPtr;

	return static_cast<mesh_handle>(std::distance(m_Sprites.begin(), invalidSlot));
}

void VulkanRenderer::UnregisterMeshRender(mesh_handle handle)
{
	if (handle < 0)
		return;
	if (m_Meshes[handle]->GetPipeline() == nullptr)
		return;
	m_Impl->RemoveMesh3D(m_Meshes[handle]->GetPipeline(), m_Meshes[handle]->GetMesh());
    m_Meshes[handle] = nullptr;
}

void VulkanRenderer::UnregisterSpriteRender(mesh_handle handle)
{
	if (handle < 0)
		return;
	if (m_Sprites[handle]->GetPipeline() == nullptr)
		return;
	m_Impl->RemoveMesh2D(m_Sprites[handle]->GetPipeline(), m_Sprites[handle]->GetMesh());
	m_Sprites[handle] = nullptr;
}

void VulkanRenderer::RegisterCamera(CameraComponent *cameraPtr)
{
    m_Cameras.push_back(cameraPtr);
}

void VulkanRenderer::UnregisterCamera(CameraComponent *cameraPtr)
{
    const auto foundIt = std::find(m_Cameras.begin(), m_Cameras.end(), cameraPtr);
    if (foundIt != m_Cameras.end())
    {
        if (m_RenderCamera == *foundIt)
            SetActiveCamera(nullptr);
        m_Cameras.erase(foundIt);
    }
}

void VulkanRenderer::SetActiveCamera(CameraComponent *cameraPtr)
{
    m_RenderCamera = cameraPtr;
    // if nullptr spawn spectactor object
}
