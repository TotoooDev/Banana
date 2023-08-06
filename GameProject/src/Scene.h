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
		m_Model.AddComponent<TransformComponent>();
		
		VertexLayout layout;
		layout.AddAttribute(Type::Vec3); // Position
		layout.AddAttribute(Type::Vec3); // Color

		std::vector<glm::vec3> positions =
		{
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(0.0f,  0.5f, 0.0f)
		};
		std::vector<glm::vec3> colors =
		{
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		Ref<VAO> vao = CreateRef<VAO>(layout, 3);
		vao->SetData(0, positions);
		vao->SetData(1, colors);

		std::vector<unsigned int> indices = { 0, 1, 2 };
		Ref<EBO> ebo = CreateRef<EBO>();
		ebo->SetData(indices);

		m_Model.AddComponent<VertexObjectComponent>(vao, ebo);


		m_Camera = CreateEntity();
		m_Camera.AddComponent<TransformComponent>();
		m_Camera.AddComponent<CameraComponent>(CreateRef<Camera>());

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
		Renderer::Get()->SetProjection(45.0f, (float)event->Width / (float)event->Height, 0.1f, 1000.0f);
	}

private:
	Entity m_Model;
	Entity m_Camera;
};