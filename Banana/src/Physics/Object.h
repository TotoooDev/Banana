#pragma once
#include <Physics/Transform.h>
#include <Physics/Collider.h>
#include <Physics/Collision.h>
#include <functional>

namespace Banana::Physics
{
	class Object
	{
	public:
		Object(Transform* transform, Collider* collider, bool isDynamic, bool isTrigger = false)
			: m_Transform(transform), m_Collider(collider), m_IsDynamic(isDynamic), m_IsTrigger(isTrigger)
		{}

		Transform* GetTransform() { return m_Transform; }
		Collider* GetCollider() { return m_Collider; }
		bool IsTrigger() { return m_IsTrigger; }
		bool IsDynamic() { return m_IsDynamic; }
		std::function<void(const Collision&, float)> GetOnCollisionCallback() { return m_OnCollision; }

		void SetTransform(Transform* transform) { m_Transform = transform; }
		void SetPosition(glm::vec3 pos) { m_Transform->Position = pos; }
		void SetCollider(Collider* collider) { m_Collider = collider; }
		void SetIsTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
		void SetOnCollisionCallback(std::function<void(const Collision&, float)> callback) { m_OnCollision = callback; }

	protected:
		Transform* m_Transform;
		Collider* m_Collider;

		bool m_IsTrigger;
		bool m_IsDynamic;

		std::function<void(const Collision&, float)> m_OnCollision;
	};
}