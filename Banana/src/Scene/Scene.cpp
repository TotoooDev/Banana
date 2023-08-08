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
				if (!vertexObject.Draw)
					continue;

				Entity ent(entity, this);
				// If the entity has a material, draw it with the material
				// If not, draw the entity with a beautiful magenta color so we know something is wrong
				if (ent.HasComponent<MaterialComponent>())
				{
					auto& materialComp = ent.GetComponent<MaterialComponent>();
					Renderer::Get()->Draw(vertexObject.ObjectVAO, vertexObject.ObjectEBO, materialComp.Materials[materialComp.UsedMaterialIndex], transform.GetTransfrom());
				}
				else
				{
					Material mat;
					mat.ColorDiffuse = glm::vec3(1.0f, 0.0f, 1.0f);
					Renderer::Get()->Draw(vertexObject.ObjectVAO, vertexObject.ObjectEBO, mat, transform.GetTransfrom());
				}
			}

			auto modelView = m_Registry.view<TransformComponent, ModelComponent, MaterialComponent>();
			for (auto&& [entity, transform, model, material] : modelView.each())
			{
				if (model.Draw)
					Renderer::Get()->Draw(model.Model, material.Materials, transform.GetTransfrom());
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
