#pragma once
#include <Wiwa/core/KeyCodes.h>

bool IsKeyDownIntr(Wiwa::KeyCode keycode);

bool IsMouseButtonPressedIntr(int button);

float GetMouseXIntr();

float GetMouseYIntr();

float GetMouseXDeltaIntr();

float GetMouseYDeltaIntr();

bool IsButtonPressedIntr(Wiwa::GamepadNum gamepad, Wiwa::KeyCode button);

float GetAxisIntr(Wiwa::GamepadNum gamepad, Wiwa::GamepadAxis axis);