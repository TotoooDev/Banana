#include <Core/Application.h>
#include <Core/Log.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Graphics/Renderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Banana
{
	Scene::~Scene()
	{
		for (auto&& [entity, script] : m_Registry.view<ScriptableComponent>().each())
		{
			auto ent = Entity(entity, this);
			script.OnEnd(ent);
		}
		OnDelete();
	}

	Entity Scene::CreateEntity()
	{
		entt::entity id = m_Registry.create();
		return Entity(id, this);
	}

	void Scene::DeleteEntity(Entity entity)
	{
		BANANA_ASSERT(entity.IsValid(), "Entity is not valid!");
		if (entity.HasComponent<ScriptableComponent>())
			entity.GetComponent<ScriptableComponent>().OnEnd(entity);
		m_Registry.destroy(entity.m_Identifier);
	}

	void Scene::UpdateScene(double timestep)
	{
		// Execute scripts first
		auto scriptView = m_Registry.view<ScriptableComponent>();
		for (auto&& [entity, script] : scriptView.each())
		{
			auto ent = Entity(entity, this);
			if (!script.HasStarted)
			{
				if (script.OnStart)
					script.OnStart(ent);
				script.HasStarted = true;
			}
			if (script.OnUpdate)
				script.OnUpdate(ent, timestep);
		}

		// Rendering
		auto camView = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto&& [camEntity, camTransform, camera] : camView.each())
		{
			Renderer::Get()->BeginScene(camera.Cam, camTransform.Translation);
			
			// Draw stuff
			auto vertexObjectView = m_Registry.view<TransformComponent, VertexObjectComponent>();
			for (auto&& [entity, transform, vertexObject] : vertexObjectView.each())
			{
				Renderer::Get()->Draw(vertexObject.ObjectVAO, vertexObject.ObjectEBO);
			}

			Renderer::Get()->EndScene();
		}

		// ImGui
		auto imguiView = m_Registry.view<ImGuiComponent>();
		for (auto&& [entity, script] : imguiView.each())
		{
			auto ent = Entity(entity, this);
			if (script.Draw)
				script.OnDraw(ent, &script.Draw, timestep);
		}
	}
}
