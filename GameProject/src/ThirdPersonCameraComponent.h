#pragma once
#include <Core/Application.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>

#include <OrbitingCamera.h>

using namespace Banana;

class ThirdPersonCameraComponent
{
public:
	ThirdPersonCameraComponent(Entity ent, Scene* scene)
	{
		ent.AddComponent<TransformComponent>();
		ent.AddComponent<CameraComponent>(CreateRef<Camera>());
		auto& camController = ent.AddComponent<OrbitingCamera>();
		camController.SetRadius(20.0f);
		auto& camScript = ent.AddComponent<ScriptableComponent>();
		camScript.OnUpdate = [&](Entity ent, Scene* scene, double timestep)
		{
			auto& camTransform = ent.GetComponent<TransformComponent>();
			auto& camController = ent.GetComponent<OrbitingCamera>();
			auto& camCam = ent.GetComponent<CameraComponent>();

			Entity model = scene->GetEntityByTag("Model");
			auto& modelTransform = model.GetComponent<TransformComponent>();

			camController.SetCenter(modelTransform.Translation);
			camController.RotateAzimuth(-m_MouseDeltaX * timestep);
			camController.RotatePolar(-m_MouseDeltaY * timestep);

			camTransform.Translation = camController.GetPosCartesian();

			camCam.Cam->Yaw = glm::degrees(camController.GetAzimuth());
			camCam.Cam->Pitch = glm::degrees(camController.GetPolar()) - 180.0f;

			m_MouseDeltaX = 0.0f;
			m_MouseDeltaY = 0.0f;

			camCam.Cam->UpdateCameraVectors();
		};

		Application::Get()->GetEventBus()->Subscribe(this, &ThirdPersonCameraComponent::OnMouseMoved);
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

		m_MouseDeltaX = event->x - m_LastMouseX;
		m_MouseDeltaY = event->y - m_LastMouseY;

		m_LastMouseX = event->x;
		m_LastMouseY = event->y;
	}

private:
	bool m_FirstMouse = true;
	float m_LastMouseX = 0.0f, m_LastMouseY = 0.0f;
	float m_MouseDeltaX = 0.0f, m_MouseDeltaY = 0.0f;

	float m_CamSensitivity = 1.0f;
};