#pragma once
#include <Graphics/RendererAPI.h>
#include <Events/Events.h>
#include <Events/EventBus.h>
#include <Physics/DynamicsWorld.h>
#include <entt/entt.hpp>
#include <string>

namespace Banana
{
	class Entity;

	class Scene
	{
	public:
		virtual ~Scene();

		Entity CreateEntity();
		void DeleteEntity(Entity entity);

		void UpdateScene(Ref<RendererAPI> renderer, double timestep);
		virtual void OnSetAsCurrent() {}
		virtual void OnReplaced() {} // Not the best name; is called when another scene is set as the current scene
		virtual void OnDelete() {}

	protected:
		Ref<Physics::DynamicsWorld> m_PhysicsWorld = nullptr;

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}
