#include "wipch.h"
#include "embed_input_functions.h"
#include <Wiwa/core/Input.h>

bool IsKeyDownIntr(Wiwa::KeyCode keycode)
{
	return Wiwa::Input::IsKeyPressed(keycode);
}

bool IsMouseButtonPressedIntr(int button)
{
	return Wiwa::Input::IsMouseButtonPressed(button);
}

float GetMouseXIntr()
{
	return Wiwa::Input::GetMouseX();
}

float GetMouseYIntr()
{
	return Wiwa::Input::GetMouseY();
}

float GetMouseXDeltaIntr()
{
	return Wiwa::Input::GetMouseXDelta();
}

float GetMouseYDeltaIntr()
{
	return Wiwa::Input::GetMouseYDelta();
}