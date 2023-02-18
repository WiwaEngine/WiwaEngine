#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiText.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>
#include <ft2build.h>
#include <freetype/freetype.h>
namespace Wiwa
{
	GuiText::GuiText(unsigned int id, Rect2i bounds, const char* string_text) : GuiControl(GuiControlType::BUTTON, id)
	{
		this->position = bounds;
		this->texture = texture;
	
		canClick = true;
	}

	GuiText::~GuiText()
	{

	}

	bool GuiText::Update()
	{
		if (state != GuiControlState::DISABLED)
		{
			// L14: TODO 3_D: Update the state of the GUiButton according to the mouse position
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
					//cout << "Pressed " << endl;
				}
				else if (Wiwa::Input::IsMouseButtonPressed(0))
				{
					state = GuiControlState::SELECTED;
					//cout << "Selected " << endl;
					//NotifyObserver();
				}

			}
			else
			{
				state = GuiControlState::NORMAL;
			}

		}

		return false;
	}

	bool GuiText::Draw(Renderer2D* render)
	{
		// Draw the right button depending on state
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();
		Vector2i positionForUpdate_1;

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture2(texture, position.x, position.y, NULL); <-- Old way to do it (example)
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		case GuiControlState::NORMAL:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;
		case GuiControlState::PRESSED:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);
		}break;
		default:
			break;
		}

		return false;
	}
}