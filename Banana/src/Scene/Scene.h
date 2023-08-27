#pragma once
#include <Graphics/RendererAPI.h>
#include <Events/Events.h>
#include <Events/EventBus.h>
#include <Physics/PhysicsWorld.h>
#include <entt/entt.hpp>
#include <string>

namespace Banana
{
	class Entity;

	class Scene
	{
	public:
		virtual ~Scene();

		Entity CreateEntity(const std::string& tag = "");
		void DeleteEntity(Entity entity);

		Entity GetEntityByTag(const std::string& tag); // Just a for loop over all the tag components so it *might* be a little bit slow :)

		void UpdateScene(Ref<RendererAPI> renderer, double timestep);
		virtual void OnSetAsCurrent() {}
		virtual void OnReplaced() {} // Not the best name; is called when another scene is set as the current scene
		virtual void OnDelete() {}

	protected:
		Ref<PhysicsWorld> m_PhysicsWorld = nullptr;

	private:
		void ScriptComponents(double timestep);
		void Physics(double timestep);
		void Rendering(Ref<RendererAPI> renderer);
		void ImGui(double timestep);

		void SetLights(Ref<RendererAPI> renderer);
		void DrawVAOs(Ref<RendererAPI> renderer);
		void DrawMeshes(Ref<RendererAPI> renderer);
		void DrawModels(Ref<RendererAPI> renderer);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}
