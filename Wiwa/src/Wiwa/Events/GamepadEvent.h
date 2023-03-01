#pragma once

#include "Event.h"
#include <Wiwa/core/KeyCodes.h>

namespace Wiwa {
	class WI_API GamepadEvent : public Event
	{
	public:
		inline GamepadNum GetGamepadIndx() const { return m_Gamepad; }

		EVENT_CLASS_CATEGORY(EventCategoryGamepad | EventCategoryInput)
	protected:
		GamepadEvent(GamepadNum gamepadIndx) : m_Gamepad(gamepadIndx) {}
		
		GamepadNum m_Gamepad;
	};


	class WI_API GamepadConnectEvent : public GamepadEvent
	{
	public:
		GamepadConnectEvent(GamepadNum gamepad) : GamepadEvent(gamepad) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Gamepad connected: " << m_Gamepad;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GamepadConnect)
	};


	class WI_API GamepadDisconnectEvent : public GamepadEvent
	{
	public:
		GamepadDisconnectEvent(GamepadNum gamepad) : GamepadEvent(gamepad) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Gamepad disconected: " << m_Gamepad;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GamepadDisconnect)
	};
}
