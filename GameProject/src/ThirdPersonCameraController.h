#pragma once
#include <Core/Application.h>
#include <Events/Events.h>

using namespace Banana;

struct ThirdPersonCameraController
{
	float DistanceFromPlayer = 20.0f;
	float AngleAroundPlayer = 0.0f;
	float Pitch = 20.0f;

	ThirdPersonCameraController()
	{
		Application::Get()->GetEventBus()->Subscribe(this, &ThirdPersonCameraController::OnMouseMoved);
	}

private:
	void OnMouseMoved(MouseMovedEvent* event)
	{
		if (m_FirstMouse)
		{
			m_LastMouseX = event->x;
			m_LastMouseY = event->y;
			m_FirstMouse = false;
			return;
		}

		m_MouseDeltaX = (event->x - m_LastMouseX) * Application::Get()->GetTimestep();
		m_MouseDeltaY = (event->y - m_LastMouseY) * Application::Get()->GetTimestep();

		m_LastMouseX = event->x;
		m_LastMouseY = event->y;

		Pitch += m_MouseDeltaY;
		AngleAroundPlayer += m_MouseDeltaX;
	}
	
private:
	bool m_FirstMouse = true;
	float m_LastMouseX = 0.0f, m_LastMouseY = 0.0f;
	float m_MouseDeltaX = 0.0f, m_MouseDeltaY = 0.0f;
};