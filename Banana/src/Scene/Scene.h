#pragma once
#include <Events/Events.h>
#include <Events/EventBus.h>
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

		void UpdateScene(double timestep);
		virtual void OnSetAsCurrent() {}
		virtual void OnReplaced() {} // Not the best name; is called when another scene is set as the current scene
		virtual void OnDelete() {}

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}
