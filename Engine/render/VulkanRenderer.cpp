#include <stdexcept>
#include <cstring>

#include "VulkanRenderer.h"
#include "vulkan/VulkanBase.h"

using namespace mk;

VulkanRenderer::VulkanRenderer(int width, int height)
{
}

void VulkanRenderer::Init(int width, int height)
{
	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);
	m_Width = width;
	m_Height = height;

	if (m_Window == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	m_Renderer = SDL_CreateRenderer(m_Window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	GUI::GetInstance().Init(m_Window, GetSDLRenderer());
}

void VulkanRenderer::Update()
{
	GUI::GetInstance().Update();

	if (!m_DepthChanged)
		return;

	m_Renderers.sort([](MeshComponent* a, MeshComponent* b)
		{
			return a->GetRenderDepth() < b->GetRenderDepth();
		});

	m_DepthChanged = false;
}

void VulkanRenderer::Render() const
{
	VulkanBase::GetInstance().
}

void VulkanRenderer::Destroy()
{
	GUI::GetInstance().Destroy();
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
	if (m_Window != nullptr)
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}
}

void VulkanRenderer::RenderRect(int x, int y, int width, int height, const Color& color) const
{
	const SDL_Rect rect{ x, m_Height - y - height, width, height };
	Color temp{};
	SDL_GetRenderDrawColor(m_Renderer, &temp.r, &temp.g, &temp.b, &temp.a);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(m_Renderer , &rect);
	SDL_SetRenderDrawColor(m_Renderer, temp.r, temp.g, temp.b, temp.a);
}

float VulkanRenderer::GetNextDepth()
{
	constexpr float depthSlice{ 0.1f };
	const float depth{ m_AutoDepth };
	m_AutoDepth += depthSlice;
	return depth;
}

SDL_Renderer* VulkanRenderer::GetSDLRenderer() const { return m_Renderer; }

int VulkanRenderer::GetHeight() const noexcept
{
	return m_Height;
}

int VulkanRenderer::GetWidth() const noexcept
{
	return m_Width;
}

void VulkanRenderer::RegisterRenderComponent(MeshComponent* renderComponentPtr)
{
	const auto foundIt = std::ranges::find(m_Renderers, renderComponentPtr);
	if (foundIt != m_Renderers.end())
		return;

	// place in right order to avoid full resort
	const float newDepth{ renderComponentPtr->GetRenderDepth() };

	const auto lowerBoundIt = std::ranges::find_if(m_Renderers,
		[newDepth](const MeshComponent* a)
		{
			const float aDepth{ a->GetRenderDepth() };
			return aDepth < newDepth;
		});
	

	if (lowerBoundIt == m_Renderers.end())
		m_Renderers.push_front(renderComponentPtr);
	else if (lowerBoundIt == m_Renderers.begin())
		m_Renderers.push_back(renderComponentPtr);
	else
		m_Renderers.insert(std::prev(lowerBoundIt), renderComponentPtr);
}

void VulkanRenderer::UnregisterRenderComponent(MeshComponent* renderComponentPtr)
{
	std::erase(m_Renderers, renderComponentPtr);
}

void VulkanRenderer::FlagDepthDirty()
{
	m_DepthChanged = true;
}

void VulkanRenderer::RenderTexture(const MeshComponent* renderComponentPtr) const
{
	const Texture2D& texture{ *renderComponentPtr->GetTexture() };
	const glm::vec2& anchor{ renderComponentPtr->GetAnchor() };
	const glm::vec2 position{ renderComponentPtr->GetRenderPosition() };
	const auto dstRect{ GetDstRect(texture, position.x, position.y) };
	const float angle{ -renderComponentPtr->GetOwner()->GetRotation() };
	const auto flipAxis{ renderComponentPtr->IsFlipped() };

	const SDL_Point pivot{
		static_cast<int>(dstRect.w * anchor.x),
		dstRect.h - static_cast<int>(dstRect.h * anchor.y) // flip Y to be at the bottom
	};

	SDL_RendererFlip flip{};
	if (flipAxis.first)
		flip = SDL_FLIP_HORIZONTAL;
	if (flipAxis.second)
		flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);

	SDL_RenderCopyEx(m_Renderer, texture.GetSDLTexture(), nullptr, &dstRect, angle, &pivot, flip);
}

SDL_Rect VulkanRenderer::GetDstRect(const Texture2D& texture, float x, float y) const
{
	const auto size{ texture.GetSize() };
	return{	static_cast<int>(x),
				m_Height - static_cast<int>(y) - texture.GetSize().y, // flip Y to be at the bottom
				size.x,
				size.y };
}
