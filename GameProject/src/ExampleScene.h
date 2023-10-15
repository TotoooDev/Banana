#include <Core/Application.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Scene/Parser.h>
#include <Graphics/Primitives/Plane.h>

#include <OrbitingCamera.h>
#include <BananaComponent.h>
#include <ThirdPersonCameraComponent.h>

using namespace Banana;

class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
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

		Parser parser;
		parser.ExportToJSON("scene.banana", this);

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
