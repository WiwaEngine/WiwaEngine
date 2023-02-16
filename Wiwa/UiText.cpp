#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiText.h"

namespace Wiwa
{
	GuiText::GuiText(unsigned int id, Rect2i bounds, Image* texture) : GuiControl(GuiControlType::BUTTON, id)
	{
		this->bounds = bounds;
		this->texture = texture;
		canClick = true;
	}

	GuiText::~GuiText()
	{

	}

	bool GuiText::Update(float dt)
	{
		if (state != GuiControlState::DISABLED)
		{
			// L14: TODO 3_D: Update the state of the GUiButton according to the mouse position
			float mouseX, mouseY;
			mouseX = Wiwa::Input::GetMouseX();
			mouseY = Wiwa::Input::GetMouseY();

			if ((mouseX > bounds.x && mouseX < (bounds.x + bounds.width)) &&
				(mouseY > bounds.y && mouseY < bounds.y + bounds.height))
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
		switch (state)
		{

		case GuiControlState::DISABLED:
		{

			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;

		case GuiControlState::NORMAL:
		{

			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{

			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;
		case GuiControlState::PRESSED:
		{

			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		}break;
		default:
			break;
		}

		return false;
	}
}