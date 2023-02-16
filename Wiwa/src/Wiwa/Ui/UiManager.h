#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>

namespace Wiwa
{
	class WI_API GuiManager
	{
	private:

		// Constructor
		GuiManager();

		// Destructor
		virtual ~GuiManager();

	public:
		// Init audio engine
		static bool Init();

		// Update audio engine events
		bool Update();

		// Terminate audio engine
		static bool Terminate();

		bool Draw();

		// Called before quitting
		bool CleanUp();

		// Additional methods
		GuiControl* CreateGuiControl(GuiControlType type,unsigned int id,Rect2i bounds,Image* texture, Image* sliderTexture, Rect2i sliderBounds);
		void DestroyGuiControl(int id);
		void AddGuiControl(GuiControl* entity);

	public:

		List<GuiControl*> controls;

		float accumulatedTime = 0.0f;
		float updateMsCycle = 0.0f;
		bool doLogic = false;

		Image* texture;

	};
}