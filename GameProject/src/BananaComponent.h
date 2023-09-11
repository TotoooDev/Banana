#pragma once
#include <Core/Application.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>

#include <OrbitingCamera.h>

using namespace Banana;

class BananaComponent
{
public:
	BananaComponent(Entity ent, Scene* scene)
	{
		RigidBody sphereRigidBody = scene->GetPhysicsWorld()->CreateRigidBody(RigidBodyType::Dynamic);
		Collider sphereCollider = sphereRigidBody.AddSphereCollider(1.0f);
		sphereCollider.SetFriction(10.0f);
		sphereRigidBody.ApplyForce(glm::vec3(-100.0f, 200.0f, 100.0f));

		Ref<Model> bananaModel = CreateRef<Model>("Banana.fbx");
		std::vector<Material> bananaMaterials = bananaModel->LoadMaterials();

		ent.AddComponent<TransformComponent>();
		ent.AddComponent<ModelComponent>(bananaModel);
		ent.AddComponent<MaterialComponent>(bananaMaterials);
		ent.AddComponent<PhysicsComponent>(sphereRigidBody);
		auto& modelScript = ent.AddComponent<ScriptableComponent>();
		modelScript.OnUpdate = [&](Entity ent, Scene* scene, double timestep)
		{
			auto& physics = ent.GetComponent<PhysicsComponent>();

			Entity cam = scene->GetEntityByTag("Camera");
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

		Application::Get()->GetEventBus()->Subscribe(this, &BananaComponent::OnKeyDown);
		Application::Get()->GetEventBus()->Subscribe(this, &BananaComponent::OnKeyUp);
	}

private:
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

private:
	struct KeysDown
	{
		bool Up = false;
		bool Down = false;
		bool Right = false;
		bool Left = false;
		bool Space = false;
	} m_KeysDown;
};