#pragma once

#include <Scene/Scene.h>
#include <Core/Log.h>
#include <entt/entt.hpp>

namespace Banana
{
	class Entity
	{
	public:
		Entity(entt::entity identifier = entt::null, Scene* scene = nullptr)
			: m_Identifier(identifier), m_Scene(scene)
		{

		}

		entt::entity GetID() const { return m_Identifier; }
		Scene* GetParentScene() const { return m_Scene; }

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
				return m_Scene->m_Registry.get<T>(m_Identifier);
			return m_Scene->m_Registry.emplace<T>(m_Identifier, std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_Identifier);
		}

		template <typename T>
		T& GetComponent()
		{
			BANANA_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_Identifier);
		}

		template <typename T>
		void RemoveComponent()
		{
			BANANA_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_Identifier);
		}

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