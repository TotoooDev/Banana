#pragma once
#include <Physics/Collider.h>
#include <Physics/Algo.h>

namespace Banana::Physics
{
	struct SphereCollider : public Collider
	{
		glm::vec3 Center = glm::vec3(0.0f);
		float Radius;

		SphereCollider(float radius = 1.0f) : Radius(radius) {}

		virtual CollisionPoints TestCollision(const Transform* transform, const Collider* collider, const Transform* colliderTransform) const override
		{
			return collider->TestCollision(colliderTransform, this, transform);
		}
		virtual CollisionPoints TestCollision(const Transform* transform, const SphereCollider* collider, const Transform* colliderTransform) const override
		{
			return Algo::FindSphereSphereCollisionPoints(this, transform, collider, colliderTransform);
		}
		virtual CollisionPoints TestCollision(const Transform* transform, const PlaneCollider* collider, const Transform* colliderTransform) const override
		{
			return Algo::FindSpherePlaneCollisionPoints(this, transform, collider, colliderTransform);
		}
	};
}