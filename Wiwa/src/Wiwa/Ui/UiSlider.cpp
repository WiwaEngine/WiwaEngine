#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiSlider.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiSlider::GuiSlider(unsigned int id, Rect2i bounds, Rect2i sliderBounds, const char* path, const char* slider_path) : GuiControl(GuiControlType::SLIDER, id)
	{
		this->position = bounds;
		this->extraPosition = sliderBounds;
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		textId2 = Wiwa::Resources::Load<Wiwa::Image>(slider_path);
		extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);
		id_quad2 = r2d.CreateInstancedQuadTex(extraTexture->GetTextureId(), extraTexture->GetSize(), { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::CENTER);


		textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		id_quad = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);

		
		state = GuiControlState::NORMAL;
		//canClick = true;
	}

	GuiSlider::~GuiSlider()
	{

	}

	bool GuiSlider::Update()
	{
		if (state != GuiControlState::DISABLED)
		{
			if (canClick == true)
			{
				float mouseX, mouseY;
				mouseX = Wiwa::Input::GetMouseX();
				mouseY = Wiwa::Input::GetMouseY();

				if ((mouseX > position.x && mouseX < (position.x + position.width)) &&
					(mouseY > position.y && mouseY < position.y + position.height))
				{
					state = GuiControlState::FOCUSED;
					if (Wiwa::Input::IsMouseButtonPressed(0))
					{
						state = GuiControlState::PRESSED;
						extraPosition.x = mouseX;
					}
					else
					{
						state = GuiControlState::NORMAL;
						/*NotifyObserver();*/
					}
				}
			}
		}

		return false;
	}

	bool GuiSlider::Draw(Renderer2D* render)
	{
		// Draw the right button depending on state
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture2(texture, position.x, position.y, NULL); <-- Old way to do it (example)
			//render->DrawTexture2(textureForSlider, extraPosition.x, extraPosition.y, NULL); <-- Old way to do it (example)
			r2d_1.UpdateInstancedQuadTex(id_quad2, { extraPosition.x,extraPosition.y }, Wiwa::Renderer2D::Pivot::CENTER);
			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::CENTER);

			
		} break;

		case GuiControlState::NORMAL:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad2, { extraPosition.x,extraPosition.y }, Wiwa::Renderer2D::Pivot::CENTER);

			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::CENTER);

		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad2, { extraPosition.x,extraPosition.y }, Wiwa::Renderer2D::Pivot::CENTER);

			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::CENTER);

		} break;
		case GuiControlState::PRESSED:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad2, { extraPosition.x,extraPosition.y }, Wiwa::Renderer2D::Pivot::CENTER);


			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::CENTER);


		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad2, { extraPosition.x,extraPosition.y }, Wiwa::Renderer2D::Pivot::CENTER);


			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::CENTER);

		}break;
		default:
			break;
		}

		return false;
	}
}