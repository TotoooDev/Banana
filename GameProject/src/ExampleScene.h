#include <Core/Application.h>
#include <Core/Timer.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Graphics/Primitives/Plane.h>
#include <Graphics/Primitives/Icosphere.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <OrbitingCamera.h>
#include <BananaComponent.h>
#include <ThirdPersonCameraComponent.h>

#include <Lua/Script.h>

using namespace Banana;

class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
		Script script("scripts/script.lua");
		script.Run();
		BANANA_INFO("value: {}", script.GetNumber("value"));
		BANANA_INFO("other_value: {}", script.GetNumber("other_value"));

		Application::Get()->GetRenderer()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
		Application::Get()->GetWindow()->ToggleMouseLocking(true);

		m_PhysicsWorld = CreateRef<PhysicsWorld>();
		RigidBody planeRigidBody = m_PhysicsWorld->CreateRigidBody(RigidBodyType::Static, glm::vec3(0.0f, -5.0f, 0.0f));
		planeRigidBody.AddBoxCollider(glm::vec3(20.0f, 0.0f, 20.0f));

		m_Model = CreateEntity("Model");
		m_Model.AddComponent<BananaComponent>(m_Model, this);

		m_Camera = CreateEntity("Camera");
		m_Camera.AddComponent<ThirdPersonCameraComponent>(m_Camera, this);

		m_Plane = CreateEntity();
		auto& planeTransform = m_Plane.AddComponent<TransformComponent>();
		planeTransform.Translation = glm::vec3(0.0f, -5.0f, 0.0f);
		planeTransform.Scale = glm::vec3(20.0f, 1.0f, 20.0f);
		m_Plane.AddComponent<MeshComponent>(CreateRef<Plane>(1, 1));
		m_Plane.AddComponent<PhysicsComponent>(planeRigidBody);

		m_Light = CreateEntity();
		auto& lightTransform = m_Light.AddComponent<TransformComponent>();
		lightTransform.Rotation = glm::vec3(0.5f, -1.0f, 0.5f);
		m_Light.AddComponent<DirectionalLightComponent>();

		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnWindowResized);
	}

private:
	void OnWindowResized(WindowResizedEvent* event)
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, (float)event->Width / (float)event->Height, 0.1f, 100.0f);
	}

private:
	Entity m_Model;
	Entity m_Plane;
	Entity m_Camera;
	Entity m_Light;
};
