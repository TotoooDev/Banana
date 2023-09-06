#include <Core/Application.h>
#include <Core/Timer.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Graphics/Primitives/Plane.h>
#include <Graphics/Primitives/Icosphere.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <OrbitingCamera.h>

using namespace Banana;

class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
		Application::Get()->GetWindow()->ToggleMouseLocking(true);

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

		m_Model = CreateEntity("Model");
		m_Model.AddComponent<TransformComponent>();
		m_Model.AddComponent<ModelComponent>(bananaModel);
		m_Model.AddComponent<MaterialComponent>(bananaMaterials);
		m_Model.AddComponent<PhysicsComponent>(sphereRigidBody);
		auto& modelScript = m_Model.AddComponent<ScriptableComponent>();
		modelScript.OnUpdate = [&](Entity ent, double timestep)
		{
			auto& physics = ent.GetComponent<PhysicsComponent>();

			Entity cam = GetEntityByTag("Camera");
			auto& camController = cam.GetComponent<OrbitingCamera>();
			auto& camCam = cam.GetComponent<CameraComponent>();

			glm::vec3 velocity = physics.RigidBody.GetVelocity();
			float speed = 3.0f;
			float maxSpeed = 5.0f;
		
			// Adding velocity
			if (m_KeysDown.Up)
				physics.RigidBody.AddVelocity(glm::vec3(camCam.Cam->GetFrontVector().x * speed, 0.0f, camCam.Cam->GetFrontVector().z * speed));
			if (m_KeysDown.Down)
				physics.RigidBody.AddVelocity(-glm::vec3(camCam.Cam->GetFrontVector().x * speed, 0.0f, camCam.Cam->GetFrontVector().z * speed));
			if (m_KeysDown.Right)
				physics.RigidBody.AddVelocity(glm::vec3(camCam.Cam->GetRightVector().x * speed, 0.0f, camCam.Cam->GetRightVector().z * speed));
			if (m_KeysDown.Left)
				physics.RigidBody.AddVelocity(-glm::vec3(camCam.Cam->GetRightVector().x * speed, 0.0f, camCam.Cam->GetRightVector().z * speed));
			if (m_KeysDown.Space)
				physics.RigidBody.AddVelocity(glm::vec3(0.0f, speed, 0.0f));

			velocity = physics.RigidBody.GetVelocity();
		
			// Capping velovity
			if (velocity.x > maxSpeed)
				velocity.x = maxSpeed;
			if (velocity.x < -maxSpeed)
				velocity.x = -maxSpeed;
			if (velocity.y > maxSpeed)
				velocity.y = maxSpeed;
			if (velocity.y < -maxSpeed)
				velocity.y = -maxSpeed;
			if (velocity.z > maxSpeed)
				velocity.z = maxSpeed;
			if (velocity.z < -maxSpeed)
				velocity.z = -maxSpeed;
			physics.RigidBody.SetVelocity(velocity);
		};

		m_Plane = CreateEntity();
		auto& planeTransform = m_Plane.AddComponent<TransformComponent>();
		planeTransform.Translation = glm::vec3(0.0f, -5.0f, 0.0f);
		planeTransform.Scale = glm::vec3(20.0f, 1.0f, 20.0f);
		m_Plane.AddComponent<MeshComponent>(CreateRef<Plane>(1, 1));
		m_Plane.AddComponent<PhysicsComponent>(planeRigidBody);

		m_Camera = CreateEntity("Camera");
		m_Camera.AddComponent<TransformComponent>();
		m_Camera.AddComponent<CameraComponent>(CreateRef<Camera>());
		auto& camController = m_Camera.AddComponent<OrbitingCamera>();
		auto& camImGui = m_Camera.AddComponent<ImGuiComponent>();
		camImGui.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();
			auto& cam = ent.GetComponent<CameraComponent>();
			auto& camController = ent.GetComponent<OrbitingCamera>();
			bool updateCamera = false;

			float radius = camController.GetRadius();

			ImGui::Begin("Camera");
			ImGui::DragFloat("Sensitivity", &m_CamSensitivity, 0.1f);
			ImGui::DragFloat("Radius", &radius, 0.1f);
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Translation), 0.1f);
			updateCamera |= ImGui::DragFloat("Yaw", &cam.Cam->Yaw, 0.1f);
			updateCamera |= ImGui::DragFloat("Pitch", &cam.Cam->Pitch, 0.1f);
			updateCamera |= ImGui::DragFloat("Roll", &cam.Cam->Roll, 0.1f);
			ImGui::End();

			camController.SetRadius(radius);

			if (updateCamera)
				cam.Cam->UpdateCameraVectors();
		};
		auto& camScript = m_Camera.AddComponent<ScriptableComponent>();
		camScript.OnUpdate = [&](Entity ent, double timestep)
		{
			auto& camTransform = ent.GetComponent<TransformComponent>();
			auto& camController = ent.GetComponent<OrbitingCamera>();
			auto& camCam = ent.GetComponent<CameraComponent>();

			Entity model = GetEntityByTag("Model");
			auto& modelTransform = model.GetComponent<TransformComponent>();

			camController.SetCenter(modelTransform.Translation);
			camController.RotateAzimuth(-m_MouseDeltaX * timestep);
			camController.RotatePolar(-m_MouseDeltaY * timestep);

			camTransform.Translation = camController.GetPosCartesian();

			camCam.Cam->Yaw = glm::degrees(camController.GetAzimuth());
			camCam.Cam->Pitch = glm::degrees(camController.GetPolar()) -180.0f;

			m_MouseDeltaX = 0.0f;
			m_MouseDeltaY = 0.0f;

			camCam.Cam->UpdateCameraVectors();
		};

		m_Light = CreateEntity();
		auto& lightTransform = m_Light.AddComponent<TransformComponent>();
		lightTransform.Rotation = glm::vec3(1.0f, -1.0f, 1.0f);
		m_Light.AddComponent<DirectionalLightComponent>();
		auto& lightImGui = m_Light.AddComponent<ImGuiComponent>();
		lightImGui.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();

			ImGui::Begin("Directional Light");
			ImGui::DragFloat3("Direction", glm::value_ptr(transform.Rotation), 0.01f, -1.0f, 1.0f);
			ImGui::End();
		};

		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnWindowResized);
		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnKeyDown);
		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnKeyUp);
		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnMouseMoved);
	}

private:
	void OnWindowResized(WindowResizedEvent* event)
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, (float)event->Width / (float)event->Height, 0.1f, 100.0f);
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
		if (event->Keycode == BANANA_KEY_ESCAPE)
			Application::Get()->Stop();
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

	bool m_FirstMouse = true;
	float m_LastMouseX = 0.0f, m_LastMouseY = 0.0f;
	float m_MouseDeltaX = 0.0f, m_MouseDeltaY = 0.0f;

	float m_CamSensitivity = 1.0f;
};