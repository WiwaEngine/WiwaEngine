#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>

namespace Wiwa
{
	class WI_API GuiCheckbox : public GuiControl
	{
	public:

		GuiCheckbox(unsigned int id, Rect2i bounds,const char* path);
		virtual ~GuiCheckbox();

		bool Update();
		bool Draw(Renderer2D* render);

		bool canClick = true;
	};
}