#include <Core/Application.h>
#include <Core/Timer.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Banana;

class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
		Renderer::Get()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		m_Model = CreateEntity();
		auto& modelTransform = m_Model.AddComponent<TransformComponent>();
		modelTransform.Rotation.y = 180.0f;
		modelTransform.Scale = glm::vec3(0.1f);
		m_Model.AddComponent<MeshComponent>(CreateRef<Model>("Banana.fbx", true));
		auto& scriptComp = m_Model.AddComponent<ScriptableComponent>();
		scriptComp.OnUpdate = [&](Entity ent, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();
			transform.Rotation.y = m_Timer.ElapsedMilli() * 0.1f;
		};

		m_DirLight = CreateEntity();
		auto& dirLightComp = m_DirLight.AddComponent<DirectionalLightComponent>();
		dirLightComp.Light.Direction = glm::vec3(-0.6f, -1.0f, 0.0f);
		auto& lightImguiComp = m_DirLight.AddComponent<ImGuiComponent>();
		lightImguiComp.Draw = true;
		lightImguiComp.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& light = ent.GetComponent<DirectionalLightComponent>();
			ImGui::Begin("Directional Light");
			ImGui::DragFloat3("Direction", glm::value_ptr(light.Light.Direction), 0.1f);
			ImGui::ColorEdit3("Ambient", glm::value_ptr(light.Light.Ambient), ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.Light.Diffuse), ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("Specular", glm::value_ptr(light.Light.Specular), ImGuiColorEditFlags_Float);
			ImGui::End();
		};

		m_Cam = CreateEntity();
		auto& transformComp = m_Cam.AddComponent<TransformComponent>();
		transformComp.Translation = glm::vec3(0.0f, 16.5, 70.0f);
		Camera camera;
		m_Cam.AddComponent<CameraComponent>(CreateRef<Camera>(camera));
		auto& imguiComp = m_Cam.AddComponent<ImGuiComponent>();
		imguiComp.Draw = true;
		imguiComp.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transformComp = ent.GetComponent<TransformComponent>();
			auto& camComp = ent.GetComponent<CameraComponent>();
			bool cameraVectorsUpdated = false;
			ImGui::Begin("Camera");
			ImGui::DragFloat3("Pos", glm::value_ptr(transformComp.Translation), 0.1f);
			cameraVectorsUpdated |= ImGui::DragFloat("Yaw", &camComp.Cam->Yaw, 0.1f);
			cameraVectorsUpdated |= ImGui::DragFloat("Pitch", &camComp.Cam->Pitch, 0.1f);
			cameraVectorsUpdated |= ImGui::DragFloat("Roll", &camComp.Cam->Roll, 0.1f);
			ImGui::End();
			if (cameraVectorsUpdated)
				camComp.Cam->UpdateCameraVectors();
		};

		m_Skybox = CreateEntity();
		std::vector<std::string> facePaths =
		{
			"skybox/miramar_ft.tga",
			"skybox/miramar_bk.tga",
			"skybox/miramar_up.tga",
			"skybox/miramar_dn.tga",
			"skybox/miramar_rt.tga",
			"skybox/miramar_lf.tga"
			
		};
		m_Skybox.AddComponent<SkyboxComponent>(CreateRef<Cubemap>(facePaths));

		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnWindowResied);
	}

	virtual void OnSetAsCurrent() override
	{
		m_Timer.Reset();
		BANANA_INFO("Scene1 set as current");
	}

	virtual void OnReplaced() override
	{
		BANANA_INFO("Scene1 gets replaced");
	}

private:
	void OnWindowResied(WindowResizedEvent* event)
	{
		Renderer::Get()->SetProjection(45.0f, (float)event->Width / (float)event->Height, 0.1f, 1000.0f);
	}

private:
	Entity m_Model;
	Entity m_DirLight;
	Entity m_PointLight;
	Entity m_Cam;
	Entity m_Skybox;
	Timer m_Timer;
};