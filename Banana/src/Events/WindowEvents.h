#pragma once

#include <Events/Event.h>

namespace Banana
{
	struct WindowClosedEvent : public Event
	{
		WindowClosedEvent()
		{
			m_Name = "Window Closed Event";
		}
	};

	struct WindowRestoredEvent : public Event
	{
		WindowRestoredEvent()
		{
			m_Name = "Window Restored Event";
		}
	};

	struct WindowMaximizedEvent : public Event
	{
		WindowMaximizedEvent()
		{
			m_Name = "Window Maximized Event";
		}
	};

	struct WindowMovedEvent : public Event
	{
		WindowMovedEvent(int xPos, int yPos)
			: x(xPos), y(yPos)
		{
			m_Name = "Window Moved Event";
		}

		int x, y;
	};

	struct WindowResizedEvent : public Event
	{
		WindowResizedEvent(unsigned int width, unsigned int height)
			: Width(width), Height(height)
		{
			m_Name = "Window Resized Event";
		}

		unsigned int Width, Height;
	};
}