#include <Core/Application.h>
#include <Core/Timer.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Graphics/Primitives/Plane.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Banana;

class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		m_PhysicsWorld = CreateRef<PhysicsWorld>();
		RigidBody sphereRigidBody = m_PhysicsWorld->CreateRigidBody(RigidBodyType::Dynamic);
		RigidBody planeRigidBody = m_PhysicsWorld->CreateRigidBody(RigidBodyType::Static, glm::vec3(0.0f, -5.0f, 0.0f));
		sphereRigidBody.AddSphereCollider(1.0f);
		sphereRigidBody.ApplyForce(glm::vec3(-100.0f, 200.0f, 100.0f));
		planeRigidBody.AddBoxCollider(glm::vec3(10.0f, 0.0f, 10.0f));

		Ref<Model> peachModel = CreateRef<Model>("Peach.fbx");
		Ref<Model> bananaModel = CreateRef<Model>("Banana.fbx");
		std::vector<Material> peachMaterials = peachModel->LoadMaterials();
		std::vector<Material> bananaMaterials = bananaModel->LoadMaterials();

		m_Model = CreateEntity();
		m_Model.AddComponent<TransformComponent>();
		m_Model.AddComponent<ModelComponent>(peachModel);
		m_Model.AddComponent<MaterialComponent>(peachMaterials);
		m_Model.AddComponent<PhysicsComponent>(sphereRigidBody);
		auto& modelScript = m_Model.AddComponent<ScriptableComponent>();
		modelScript.OnUpdate = [&](Entity ent, double timestep)
		{
			auto& physics = ent.GetComponent<PhysicsComponent>();
			if (m_KeysDown.Z)
				physics.RigidBody.SetVelocity(glm::vec3(0.0f, 10.0f, 0.0f));
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
		lightTransform.Rotation = glm::vec3(0.0f, -1.0f, 0.0f);
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
		if (event->Keycode == BANANA_KEY_Z)
			m_KeysDown.Z = true;
	}
	void OnKeyUp(KeyUpEvent* event)
	{
		if (event->Keycode == BANANA_KEY_Z)
			m_KeysDown.Z = false;
	}

private:
	Entity m_Model;
	Entity m_Plane;
	Entity m_Camera;
	Entity m_Light;

	struct KeysDown
	{
		bool Z = false;
	} m_KeysDown;
};