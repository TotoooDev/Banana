#pragma once

#include <Events/Event.h>

namespace Banana
{
	struct KeyDownEvent : public Event
	{
		KeyDownEvent(int keycode)
			: Keycode(keycode)
		{
			m_Name = "Key Down Event";
		}

		int Keycode;
	};

	struct KeyUpEvent : public Event
	{
		KeyUpEvent(int keycode)
			: Keycode(keycode)
		{
			m_Name = "Key Up Event";
		}

		int Keycode;
	};
}