#include <wipch.h>
#include "Renderer2D.h"

#include <Wiwa/core/Application.h>

#include <Wiwa/utilities/render/InstanceRenderer.h>
#include <Wiwa/utilities/Log.h>

#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Wiwa/render/RenderManager.h>

namespace Wiwa {
	Renderer2D::Renderer2D() {
		//IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	}

	Renderer2D::~Renderer2D()
	{
	}

	Vector2f Renderer2D::CalculateScreenGlPos(const Rect2i& rect, Pivot pivot)
	{
		Vector2f pivot_position = { static_cast<float>(rect.x),static_cast<float>(rect.y) };

		switch (pivot) {
		case Pivot::DOWNLEFT:
			pivot_position.x += rect.width / 2;
			pivot_position.y -= rect.height / 2;
			break;
		case Pivot::UPLEFT:
			pivot_position.x += rect.width / 2;
			pivot_position.y += rect.height / 2;
			break;
		}

		return pivot_position;
	}

	Vector2f Renderer2D::CalculateScreenGlPos(const Vector2i& position, const Size2i& size, Pivot pivot)
	{
		Vector2f pivot_position = { static_cast<float>(position.x), static_cast<float>(position.y) };

		switch (pivot) {
		case Pivot::DOWNLEFT:
			pivot_position.x += size.w / 2;
			pivot_position.y -= size.h / 2;
			break;
		case Pivot::UPLEFT:
			pivot_position.x += size.w / 2;
			pivot_position.y += size.h / 2;
			break;
		}

		return pivot_position;
	}

	TextureClip Renderer2D::CalculateTextureClip(const Rect2i& rect, const Size2i& texSize)
	{
		TextureClip tclip;

		Vector2f pos = {
			(float)rect.x / texSize.w,
			(float)rect.y / texSize.h
		};

		Size2f size = {
			(float)rect.width / texSize.w,
			(float)rect.height / texSize.h
		};

		tclip.topLeft = pos;

		tclip.topRight.x = pos.x + size.w;
		tclip.topRight.y = pos.y;

		tclip.bottomLeft.x = pos.x;
		tclip.bottomLeft.y = pos.y + size.h;

		tclip.bottomRight.x = pos.x + size.w;
		tclip.bottomRight.y = pos.y + size.h;

		return tclip;
	}

	bool Renderer2D::Init()
	{
		Window& window = Application::Get().GetWindow();
		Size2i& resolution = Application::Get().GetTargetResolution();

		instanceRenderer = new InstanceRenderer(MAXQUADS);
		instanceRenderer->Init("resources/shaders/instanced_tex_color");

		// Set camera
		m_ActiveCamera.SetOrthographic(resolution.w, resolution.h);
		m_ActiveCamera.setPosition({ 0.0f, 0.0f, 3.0f });

		RenderManager::SetLayerCamera(1, &m_ActiveCamera);

		WI_CORE_INFO("Renderer2D initialized");
		return true;
	}

	void Renderer2D::Update()
	{
		m_RenderCallsInstancedCount = 0;
		UpdateInstanced();
	}

	void Renderer2D::Close()
	{
		//delete instanceRenderer;
	}

	uint32_t Renderer2D::CreateInstancedQuadTex(uint32_t textureId, const Size2i& srcSize, const Vector2i& position, const Size2i& size, const Color4f& color, const Rect2i& clip, Pivot pivot)
	{
		TextureClip tclip = CalculateTextureClip(clip, srcSize);

		return instanceRenderer->AddInstance(textureId, position, size, color, tclip, pivot);
	}

	uint32_t Renderer2D::CreateInstancedQuadTex(uint32_t textureId, const Size2i& srcSize, const Vector2i& position, const Size2i& size, Pivot pivot)
	{
		TextureClip clip = {
			{ 1.0f, 1.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.0f, 1.0f }
		};

		Color4f color = Color::WHITE;

		return instanceRenderer->AddInstance(textureId, position, size, color, clip, pivot);
	}

	uint32_t Renderer2D::CreateInstancedQuadTex(uint32_t textureId, const Size2i& srcSize, const Vector2i& position, const Size2i& size, const Rect2i& clip, Pivot pivot)
	{
		TextureClip tclip = CalculateTextureClip(clip, srcSize);

		Color4f color = Color::WHITE;
		return instanceRenderer->AddInstance(textureId, position, size, color, tclip, pivot);
	}

	void Renderer2D::RemoveInstance(uint32_t instance)
	{
		instanceRenderer->RemoveInstance(instance);
	}

	void Renderer2D::EnableInstance(uint32_t instance)
	{
		instanceRenderer->EnableInstance(instance);
	}

	void Renderer2D::DisableInstance(uint32_t instance)
	{
		instanceRenderer->DisableInstance(instance);
	}

	void Renderer2D::SetInstanceEnabled(uint32_t instance, bool enabled)
	{
		instanceRenderer->SetEnabled(instance, enabled);
	}

	void Renderer2D::UpdateInstancedQuadTex(uint32_t id, const Vector2i& position, Pivot pivot)
	{
		instanceRenderer->UpdateInstance(id, position, pivot);
	}

	void Renderer2D::UpdateInstancedQuad(uint32_t id, const Vector2i& position, const Size2i& size, const Color4f& color)
	{
		instanceRenderer->UpdateInstance(id, position, size, color);
	}

	void Renderer2D::UpdateInstanced()
	{
		FrameBuffer& framebuffer = *m_ActiveCamera.frameBuffer;
		glViewport(0, 0, framebuffer.getWidth(), framebuffer.getHeight());

		framebuffer.Bind();

		instanceRenderer->Update();
		instanceRenderer->Render(m_ActiveCamera.getProjection(), m_ActiveCamera.getView());

		framebuffer.Unbind();

		m_RenderCallsInstancedCount++;
	}
}