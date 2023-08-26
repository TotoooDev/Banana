#include <Core/Application.h>
#include <Core/Timer.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Graphics/Primitives/Plane.h>
#include <Graphics/Primitives/Icosphere.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Banana;

class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

		m_PhysicsWorld = CreateRef<PhysicsWorld>();
		RigidBody sphereRigidBody = m_PhysicsWorld->CreateRigidBody(RigidBodyType::Dynamic);
		RigidBody planeRigidBody = m_PhysicsWorld->CreateRigidBody(RigidBodyType::Static, glm::vec3(0.0f, -5.0f, 0.0f));
		Collider sphereCollider = sphereRigidBody.AddSphereCollider(1.0f);
		sphereCollider.SetFriction(10.0f);
		sphereRigidBody.ApplyForce(glm::vec3(-100.0f, 200.0f, 100.0f));
		planeRigidBody.AddBoxCollider(glm::vec3(20.0f, 0.0f, 20.0f));

		Ref<Model> peachModel = CreateRef<Model>("Peach.fbx");
		Ref<Model> bananaModel = CreateRef<Model>("Banana.fbx");
		std::vector<Material> peachMaterials = peachModel->LoadMaterials();
		std::vector<Material> bananaMaterials = bananaModel->LoadMaterials();

		m_Model = CreateEntity();
		m_Model.AddComponent<TransformComponent>();
		m_Model.AddComponent<ModelComponent>(bananaModel);
		m_Model.AddComponent<MaterialComponent>(bananaMaterials);
		m_Model.AddComponent<PhysicsComponent>(sphereRigidBody);
		auto& modelScript = m_Model.AddComponent<ScriptableComponent>();
		modelScript.OnUpdate = [&](Entity ent, double timestep)
		{
			auto& physics = ent.GetComponent<PhysicsComponent>();
			glm::vec3 velocity = physics.RigidBody.GetVelocity();
			float speed = 3.0f;
			float maxSpeed = 5.0f;
		
			if (m_KeysDown.Up)
			{
				if (velocity.z > -maxSpeed)
					physics.RigidBody.AddVelocity(glm::vec3(0.0f, 0.0f, -5.0f));
				else
					physics.RigidBody.SetVelocity(glm::vec3(velocity.x, velocity.y, -maxSpeed));
			}
			if (m_KeysDown.Down)
			{
				if (velocity.z < maxSpeed)
					physics.RigidBody.AddVelocity(glm::vec3(0.0f, 0.0f, 5.0f));
				else
					physics.RigidBody.SetVelocity(glm::vec3(velocity.x, velocity.y, maxSpeed));
			}
			if (m_KeysDown.Right)
			{
				if (velocity.x < maxSpeed)
					physics.RigidBody.AddVelocity(glm::vec3(5.0f, 0.0f, 0.0f));
				else
					physics.RigidBody.SetVelocity(glm::vec3(maxSpeed, velocity.y, velocity.z));
			}
			if (m_KeysDown.Left)
			{
				if (velocity.x > -maxSpeed)
					physics.RigidBody.AddVelocity(glm::vec3(-5.0f, 0.0f, 0.0f));
				else
					physics.RigidBody.SetVelocity(glm::vec3(-maxSpeed, velocity.y, velocity.z));
			}
			if (m_KeysDown.Space)
			{
				if (velocity.y < maxSpeed)
					physics.RigidBody.AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
				else
					physics.RigidBody.SetVelocity(glm::vec3(velocity.x, maxSpeed, velocity.z));
			}
		
		};

		m_Plane = CreateEntity();
		auto& planeTransform = m_Plane.AddComponent<TransformComponent>();
		planeTransform.Translation = glm::vec3(0.0f, -5.0f, 0.0f);
		planeTransform.Scale = glm::vec3(20.0f, 1.0f, 20.0f);
		m_Plane.AddComponent<MeshComponent>(CreateRef<Plane>(1, 1));
		m_Plane.AddComponent<PhysicsComponent>(planeRigidBody);

		m_Camera = CreateEntity();
		auto& camTransform = m_Camera.AddComponent<TransformComponent>();
		camTransform.Translation = glm::vec3(0.0f, 0.0f, 20.0f);
		m_Camera.AddComponent<CameraComponent>(CreateRef<Camera>());
		auto& camImGui = m_Camera.AddComponent<ImGuiComponent>();
		camImGui.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();
			auto& cam = ent.GetComponent<CameraComponent>();
			bool updateCamera = false;

			ImGui::Begin("Camera");
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Translation), 0.1f);
			updateCamera |= ImGui::DragFloat("Yaw", &cam.Cam->Yaw, 0.1f);
			updateCamera |= ImGui::DragFloat("Pitch", &cam.Cam->Pitch, 0.1f);
			updateCamera |= ImGui::DragFloat("Roll", &cam.Cam->Roll, 0.1f);
			ImGui::End();

			if (updateCamera)
				cam.Cam->UpdateCameraVectors();
		};

		m_Light = CreateEntity();
		auto& lightTransform = m_Light.AddComponent<TransformComponent>();
		lightTransform.Rotation = glm::normalize(glm::vec3(20.0f, -50.0f, 20.0f));
		m_Light.AddComponent<DirectionalLightComponent>();

		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnWindowResized);
		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnKeyDown);
		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnKeyUp);
	}

private:
	void OnWindowResized(WindowResizedEvent* event)
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, (float)event->Width / (float)event->Height, 0.1f, 1000.0f);
	}
	void OnKeyDown(KeyDownEvent* event)
	{
		if (event->Keycode == BANANA_KEY_UP)
			m_KeysDown.Up = true;
		if (event->Keycode == BANANA_KEY_DOWN)
			m_KeysDown.Down = true;
		if (event->Keycode == BANANA_KEY_RIGHT)
			m_KeysDown.Right = true;
		if (event->Keycode == BANANA_KEY_LEFT)
			m_KeysDown.Left = true;
		if (event->Keycode == BANANA_KEY_SPACE)
			m_KeysDown.Space = true;
	}
	void OnKeyUp(KeyUpEvent* event)
	{
		if (event->Keycode == BANANA_KEY_UP)
			m_KeysDown.Up = false;
		if (event->Keycode == BANANA_KEY_DOWN)
			m_KeysDown.Down = false;
		if (event->Keycode == BANANA_KEY_RIGHT)
			m_KeysDown.Right = false;
		if (event->Keycode == BANANA_KEY_LEFT)
			m_KeysDown.Left = false;
		if (event->Keycode == BANANA_KEY_SPACE)
			m_KeysDown.Space = false;
	}

private:
	Entity m_Model;
	Entity m_Plane;
	Entity m_Camera;
	Entity m_Light;

	struct KeysDown
	{
		bool Up = false;
		bool Down = false;
		bool Right = false;
		bool Left = false;
		bool Space = false;
	} m_KeysDown;
};