#include <Core/Application.h>
#include <Core/Log.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Banana
{
	Scene::~Scene()
	{
		for (auto&& [entity, script] : m_Registry.view<ScriptableComponent>().each())
		{
			auto ent = Entity(entity, this);
			script.OnEnd(ent, this);
		}
		OnDelete();
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		entt::entity id = m_Registry.create();
		Entity ent(id, this);
		if (!tag.empty())
			ent.AddComponent<TagComponent>(tag);
		return ent;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		BANANA_ASSERT(entity.IsValid(), "Entity is not valid!");
		if (entity.HasComponent<ScriptableComponent>())
			entity.GetComponent<ScriptableComponent>().OnEnd(entity, this);
		m_Registry.destroy(entity.m_Identifier);
	}

	Entity Scene::GetEntityByTag(const std::string& tag)
	{
		auto tagView = m_Registry.view<TagComponent>();
		for (auto&& [entity, tagComp] : tagView.each())
		{
			if (tag == tagComp.Tag)
				return Entity(entity, this);
		}
		return Entity(); // Invalid entity
	}

	void Scene::UpdateScene(Ref<RendererAPI> renderer, double timestep)
	{
		ScriptComponents(timestep);
		Physics(timestep);
		Rendering(renderer);
		ImGui(timestep);
	}

	void Scene::ScriptComponents(double timestep)
	{
		auto scriptView = m_Registry.view<ScriptableComponent>();
		for (auto&& [entity, script] : scriptView.each())
		{
			auto ent = Entity(entity, this);
			if (!script.HasStarted)
			{
				if (script.OnStart)
					script.OnStart(ent, this);
				script.HasStarted = true;
			}
			if (script.OnUpdate)
				script.OnUpdate(ent, this, timestep);
		}
	}

	void Scene::Physics(double timestep)
	{
		if (!m_PhysicsWorld)
			return;
			
		m_PhysicsWorld->Step((float)timestep);

		// Interpolate rigid bodies transform
		float interpolationFactor = m_PhysicsWorld->GetAccumulator() / m_PhysicsWorld->GetTimestep();
		auto physicsView = m_Registry.view<TransformComponent, PhysicsComponent>();
		for (auto&& [entity, transform, physics] : physicsView.each())
		{
			transform.Translation = physics.RigidBody.GetPosition();
			transform.Rotation = physics.RigidBody.GetRotation();
		}
	}

	void Scene::Rendering(Ref<RendererAPI> renderer)
	{
		auto camView = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto&& [camEntity, camTransform, camera] : camView.each())
		{
			renderer->BeginScene(camera.Cam, camTransform.Translation);

			SetLights(renderer);

			DrawVAOs(renderer);
			DrawMeshes(renderer);
			DrawModels(renderer);

			renderer->RenderScene();
		}
	}

	void Scene::SetLights(Ref<RendererAPI> renderer)
	{
		auto dirLightView = m_Registry.view<TransformComponent, DirectionalLightComponent>();
		for (auto&& [entity, transform, light] : dirLightView.each())
			renderer->AddDirectionalLight(light.Light, transform.Rotation);
		auto pointLightView = m_Registry.view<TransformComponent, PointLightComponent>();
		for (auto&& [entity, transform, light] : pointLightView.each())
			renderer->AddPointLight(light.Light, transform.Translation);
		auto spotLightView = m_Registry.view<TransformComponent, SpotLightComponent>();
		for (auto&& [entity, transform, light] : spotLightView.each())
			renderer->AddSpotLight(light.Light, transform.Translation, transform.Rotation);
	}

	void Scene::DrawVAOs(Ref<RendererAPI> renderer)
	{
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
				renderer->Draw(vertexObject.ObjectVAO, vertexObject.ObjectEBO, materialComp.Materials[materialComp.UsedMaterialIndex], transform.GetTransfrom());
			}
			else
			{
				Material mat;
				mat.ColorDiffuse = glm::vec3(1.0f, 0.0f, 1.0f);
				mat.ColorSpecular = glm::vec3(1.0f, 0.0f, 1.0f);
				mat.UseColors = true;
				renderer->Draw(vertexObject.ObjectVAO, vertexObject.ObjectEBO, mat, transform.GetTransfrom());
			}
		}
	}

	void Scene::DrawMeshes(Ref<RendererAPI> renderer)
	{
		auto meshView = m_Registry.view<TransformComponent, MeshComponent>();
		for (auto&& [entity, transform, mesh] : meshView.each())
		{
			if (!mesh.Draw)
				continue;

			Entity ent(entity, this);
			// If the entity has a material, draw it with the material
			// If not, draw the entity with a beautiful magenta color so we know something is wrong
			if (ent.HasComponent<MaterialComponent>())
			{
				auto& materialComp = ent.GetComponent<MaterialComponent>();
				renderer->Draw(mesh.Mesh, materialComp.Materials[materialComp.UsedMaterialIndex], transform.GetTransfrom());
			}
			else
			{
				Material mat;
				mat.ColorDiffuse = glm::vec3(1.0f, 0.0f, 1.0f);
				mat.ColorSpecular = glm::vec3(1.0f, 0.0f, 1.0f);
				mat.UseColors = true;
				renderer->Draw(mesh.Mesh, mat, transform.GetTransfrom());
			}
		}
	}

	void Scene::DrawModels(Ref<RendererAPI> renderer)
	{
		auto modelView = m_Registry.view<TransformComponent, ModelComponent, MaterialComponent>();
		for (auto&& [entity, transform, model, material] : modelView.each())
		{
			if (model.Draw)
				renderer->Draw(model.Model, material.Materials, transform.GetTransfrom());
		}
	}

	void Scene::ImGui(double timestep)
	{
		auto imguiView = m_Registry.view<ImGuiComponent>();
		for (auto&& [entity, script] : imguiView.each())
		{
			auto ent = Entity(entity, this);
			if (script.Draw)
				script.OnDraw(ent, &script.Draw, timestep);
		}
	}
}
