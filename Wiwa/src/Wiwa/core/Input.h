#pragma once

#include <Wiwa/core/Core.h>

namespace Wiwa
{
	class WI_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }	
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static float GetMouseXDelta() { return s_Instance->GetMouseXDeltaImpl(); }
		inline static float GetMouseYDelta() { return s_Instance->GetMouseYDeltaImpl(); }
		inline static void LockCursor() { s_Instance->LockCursorImpl(); }
		inline static void UnlockCursor() { s_Instance->UnlockCursorImpl(); }
		inline static float GetAxis(int gamepadIndx, int axis) { return s_Instance->GetAxisImpl(gamepadIndx, axis); }
		inline static bool IsButtonPressed(int gamepadIndx,int keycode) { return s_Instance->IsButtonPressedImpl(gamepadIndx, keycode); }

		inline static void Update() { return s_Instance->UpdateImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual float GetMouseXDeltaImpl() = 0;
		virtual float GetMouseYDeltaImpl() = 0;
		virtual void UpdateImpl() = 0;
		virtual void LockCursorImpl() = 0;
		virtual void UnlockCursorImpl() = 0;
		virtual float GetAxisImpl(int gamepadIndx, int axis) = 0;
		virtual bool IsButtonPressedImpl(int gamepadIndx, int keycode) = 0;
	private:
		static Input* s_Instance;
	protected:
		static float prevMouseX;
		static float prevMouseY;
	};
}