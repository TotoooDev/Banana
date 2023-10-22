#pragma once
#include <Scene/Scene.h>
#include <Core/Log.h>
#include <entt/entt.hpp>

namespace Banana
{
	/**
	 * An entity is anything that lives in a Scene.
	 * You can attach components to an entity to alter its behavior.
	 */
	class Entity
	{
	public:
		/**
		 * @param identifier - The entt ID.
		 * @param scene - A pointer to the parent scene.
		 */
		Entity(entt::entity identifier = entt::null, Scene* scene = nullptr)
			: m_Identifier(identifier), m_Scene(scene)
		{

		}

		/**
		 * @returns The entt ID.
		 */
		entt::entity GetID() const { return m_Identifier; }

		/**
		 * @returns A pointer to the parent scene of the entity.
		 */
		Scene* GetParentScene() const { return m_Scene; }

		/**
		 * Attaches a component to the entity.
		 * @param args - The arguments to pass in the component constructor.
		 * @returns The new component.
		 */
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
				return m_Scene->m_Registry.get<T>(m_Identifier);
			return m_Scene->m_Registry.emplace<T>(m_Identifier, std::forward<Args>(args)...);
		}

		/**
		 * @returns Whether the entity has the component or not.
		 */
		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_Identifier);
		}

		/**
		 * @returns The component specified in the template parameter.
		 */
		template <typename T>
		T& GetComponent()
		{
			BANANA_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_Identifier);
		}

		/**
		 * Removes a component from the entity.
		 */
		template <typename T>
		void RemoveComponent()
		{
			BANANA_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_Identifier);
		}

		/**
		 * @returns Whether the entity is valid or not.
		 */
		bool IsValid()
		{
			return m_Identifier != entt::null && m_Scene != nullptr;
		}

		bool operator==(const Entity& ent) const
		{
			return this->m_Identifier == ent.m_Identifier && this->m_Scene == ent.m_Scene;
		}
		bool operator!=(const Entity& ent) const
		{
			return this->m_Identifier != ent.m_Identifier || this->m_Scene != ent.m_Scene;
		}

	private:
		entt::entity m_Identifier = entt::null;
		Scene* m_Scene = nullptr;

		friend class Scene;
	};
}