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
		Application::Get()->GetRenderer ()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		m_PhysicsWorld = CreateRef<PhysicsWorld>();

		Ref<Model> peachModel = CreateRef<Model>("Peach.fbx");
		Ref<Model> bananaModel = CreateRef<Model>("Banana.fbx");
		std::vector<Material> peachMaterials = peachModel->LoadMaterials();
		std::vector<Material> bananaMaterials = bananaModel->LoadMaterials();

		RigidBody sphereRigidBody = m_PhysicsWorld->CreateRigidBody(RigidBodyType::Dynamic);
		RigidBody planeRigidBody = m_PhysicsWorld->CreateRigidBody(RigidBodyType::Static, glm::vec3(0.0f, -5.0f, 0.0f));
		sphereRigidBody.AddSphereCollider(1.0f);
		planeRigidBody.AddBoxCollider(glm::vec3(10.0f, 0.0f, 10.0f));

		m_Model = CreateEntity();
		m_Model.AddComponent<TransformComponent>();
		m_Model.AddComponent<ModelComponent>(peachModel);
		m_Model.AddComponent<MaterialComponent>(peachMaterials);
		m_Model.AddComponent<PhysicsComponent>(sphereRigidBody);

		m_Plane = CreateEntity();
		auto& planeTransform = m_Plane.AddComponent<TransformComponent>();
		planeTransform.Translation = glm::vec3(0.0f, -5.0f, 0.0f);
		// planeTransform.Scale = glm::vec3(20.0f, 1.0f, 20.0f);
		m_Plane.AddComponent<MeshComponent>(CreateRef<Plane>(5, 5));
		m_Plane.AddComponent<PhysicsComponent>(planeRigidBody);
		auto& planeImGui = m_Plane.AddComponent<ImGuiComponent>();
		planeImGui.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();
		
			ImGui::Begin("Plane");
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Translation), 0.1f);
			ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);
			ImGui::End();
		};

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

		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnWindowResied);
	}

	virtual void OnSetAsCurrent() override
	{
		BANANA_INFO("Scene1 set as current");
	}

	virtual void OnReplaced() override
	{
		BANANA_INFO("Scene1 gets replaced");
	}

private:
	void OnWindowResied(WindowResizedEvent* event)
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, (float)event->Width / (float)event->Height, 0.1f, 1000.0f);
	}

private:
	Entity m_Model;
	Entity m_Plane;
	Entity m_Camera;
	Entity m_Light;
};