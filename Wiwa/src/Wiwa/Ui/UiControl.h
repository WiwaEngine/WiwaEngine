#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>

namespace Wiwa
{
	enum class GuiControlType
	{
		BUTTON,
		TOGGLE,
		CHECKBOX,
		SLIDER,
		INPUTBOX
	};

	enum class GuiControlState
	{
		DISABLED,
		NORMAL,
		FOCUSED,
		PRESSED,
		SELECTED
	};

	class WI_API GuiControl
	{
	public:

		GuiControl(GuiControlType type, unsigned int id) : type(type), id(id), state(GuiControlState::NORMAL) {}

		GuiControl(GuiControlType type, Rect2i bounds, char text[32]) :type(type), state(GuiControlState::NORMAL), bounds(bounds), text(text)
		{
			//color.r = 255; color.g = 255; color.b = 255;
			texture = nullptr;
		}
		



		virtual bool Update(float dt)
		{
			return true;
		}

		virtual bool Draw(Renderer2D* render)
		{
			return true;
		}

		void SetTexture(Image* tex)
		{
			texture = tex;

		}
		void SetSliderTexture(Image* tex)
		{
			textureForSlider = tex;

		}

	public:

		bool SelectedForDrawing;
		int id;
		GuiControlType type;
		GuiControlState state;

		char text[32];           // Control text (if required)
		Rect2i bounds;        // Position and size
		Rect2i extraBounds;
		//SDL_Color color;        // Tint color

		Image* texture;
		Image* textureForSlider;	// Texture atlas reference
		Rect2i section;       // Texture atlas base section

		//Font font;              // Text font

		//Module* observer;        // Observer module (it should probably be an array/list)
	};
}