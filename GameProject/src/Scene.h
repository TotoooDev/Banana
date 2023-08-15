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
		Application::Get()->GetRenderer ()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		m_Model = CreateEntity();
		m_Model.AddComponent<TransformComponent>();
		auto& modelComponent = m_Model.AddComponent<ModelComponent>(CreateRef<Model>("Peach.fbx"));
		m_Model.AddComponent<MaterialComponent>(modelComponent.Model->LoadMaterials());
		auto& modelImGui = m_Model.AddComponent<ImGuiComponent>();
		modelImGui.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();
			ImGui::Begin("Model");
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Translation), 0.1f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);
			ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);
			ImGui::End();
		};

		m_Camera = CreateEntity();
		m_Camera.AddComponent<TransformComponent>();
		m_Camera.AddComponent<CameraComponent>(CreateRef<Camera>());
		auto& camImGui = m_Camera.AddComponent<ImGuiComponent>();
		camImGui.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();
			ImGui::Begin("Camera");
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Translation), 0.1f);
			ImGui::End();
		};

		m_Light = CreateEntity();
		m_Light.AddComponent<TransformComponent>();
		m_Light.AddComponent<DirectionalLightComponent>();
		auto& lightImGui = m_Light.AddComponent<ImGuiComponent>();
		lightImGui.OnDraw = [&](Entity ent, bool* isOpen, double timestep)
		{
			auto& transform = ent.GetComponent<TransformComponent>();
			ImGui::Begin("Directional Light");
			ImGui::DragFloat3("Direction", glm::value_ptr(transform.Rotation), 0.01f);
			ImGui::End();
		};

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
	Entity m_Camera;
	Entity m_Light;
};