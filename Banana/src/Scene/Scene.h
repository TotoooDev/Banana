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

	/**
	 * Represents a scene that holds entities and a physics world.
	 */
	class Scene
	{
	public:
		virtual ~Scene();

		/**
		 * @returns A new entity.
		 * @param tag - The name of the new entity.
		 */
		Entity CreateEntity(const std::string& tag = "");

		/**
		 * @param entity - The entity to delete.
		 */
		void DeleteEntity(Entity entity);

		/**
		 * Just a for loop over all the tag components so it *might* be a little bit slow :)
		 * @returns The entity with the specified tag if found, an invalid entity if not.
		 * @param tag - The name to search for.
		 */
		Entity GetEntityByTag(const std::string& tag);

		/**
		 * @returns The PhysicsWorld of the scene.
		 */
		Ref<PhysicsWorld> GetPhysicsWorld() { return m_PhysicsWorld; }

		/**
		 * Performs operations on all the entities according to their components.
		 * @param renderer - The RendererAPI to use to render the scene.
		 * @param timestep - The timestep to use for updating.
		 */
		void UpdateScene(Ref<RendererAPI> renderer, double timestep);

		/**
		 * Gets called when the scene is set as the current scene.
		 */
		virtual void OnSetAsCurrent() {}

		/**
		 * Gets called when the scene gets replaced by another scene.
		 */
		virtual void OnReplaced() {}

		/**
		 * Gets called when the scene gets deleted.
		 */
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
		friend class Parser;
	};
}
