#include <Core/Application.h>
#include <Core/Timer.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Physics/DynamicsWorld.h>
#include <Physics/Colliders/SphereCollider.h>
#include <Physics/Solvers/PositionSolver.h>
#include <Physics/Solvers/ImpulseSolver.h>
#include <Graphics/Primitives/Icosphere.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Banana;

class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
		Application::Get()->GetRenderer ()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		Ref<Physics::RigidBody> rigidBody1 = CreateRef<Physics::RigidBody>(new Physics::Transform, new Physics::SphereCollider);
		Ref<Physics::RigidBody> rigidBody2 = CreateRef<Physics::RigidBody>(new Physics::Transform, new Physics::SphereCollider, 2.0f);
		rigidBody2->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
		rigidBody2->SetVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
		Ref<Physics::Object> rigidBody3 = CreateRef<Physics::Object>(new Physics::Transform, new Physics::SphereCollider(10.0f), false);
		rigidBody3->SetPosition(glm::vec3(-5.0f, -20.0f, 0.0f));

		m_PhysicsWorld = CreateRef<Physics::DynamicsWorld>();
		m_PhysicsWorld->AddRigidBody(rigidBody1);
		m_PhysicsWorld->AddRigidBody(rigidBody2);
		m_PhysicsWorld->AddObject(rigidBody3);
		m_PhysicsWorld->AddSolver(CreateRef<Physics::ImpulseSolver>());

		Ref<Model> peachModel = CreateRef<Model>("Peach.fbx");
		std::vector<Material> peachMaterials = peachModel->LoadMaterials();
		Ref<Model> bananaModel = CreateRef<Model>("Banana.fbx");
		std::vector<Material> bananaMaterials = bananaModel->LoadMaterials();

		m_Model1 = CreateEntity();
		m_Model1.AddComponent<TransformComponent>();
		m_Model1.AddComponent<ModelComponent>(peachModel);
		m_Model1.AddComponent<MaterialComponent>(peachMaterials);
		m_Model1.AddComponent<RigidBodyComponent>(rigidBody1);

		m_Model2 = CreateEntity();
		m_Model2.AddComponent<TransformComponent>();
		m_Model2.AddComponent<ModelComponent>(bananaModel);
		m_Model2.AddComponent<MaterialComponent>(bananaMaterials);
		m_Model2.AddComponent<RigidBodyComponent>(rigidBody2);

		m_Model3 = CreateEntity();
		auto& model3Transform = m_Model3.AddComponent<TransformComponent>();
		model3Transform.Translation = glm::vec3(-5.0f, -20.0f, 0.0f);
		model3Transform.Scale = glm::vec3(10.0f);
		m_Model3.AddComponent<MeshComponent>(CreateRef<Icosphere>(2));
		m_Model3.AddComponent<PhysicsObjectComponent>(rigidBody3);
		m_Model3.AddComponent<PointLightComponent>();
		Material mat;
		mat.ColorDiffuse = glm::vec3(1.0f);
		mat.UseColors = true;
		mat.DrawWireframe = true;
		std::vector<Material> materials = { mat };
		m_Model3.AddComponent<MaterialComponent>(materials);

		m_Camera = CreateEntity();
		auto& camTransform = m_Camera.AddComponent<TransformComponent>();
		camTransform.Translation = glm::vec3(0.0f, -20.0f, 40.0f);
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
	Entity m_Model1;
	Entity m_Model2;
	Entity m_Model3;
	Entity m_Camera;
	Entity m_Light;
};