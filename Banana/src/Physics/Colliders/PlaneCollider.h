#pragma once
#include <Physics/Collider.h>
#include <Physics/Algo.h>

namespace Banana::Physics
{
	struct PlaneCollider : public Collider
	{
		glm::vec3 Plane;
		float Distance;

		virtual CollisionPoints TestCollision(const Transform* transform, const Collider* collider, const Transform* colliderTransform) const override
		{
			return collider->TestCollision(colliderTransform, this, transform);
		}
		virtual CollisionPoints TestCollision(const Transform* transform, const SphereCollider* collider, const Transform* colliderTransform) const override
		{
			CollisionPoints points =  collider->TestCollision(colliderTransform, this, transform);

			glm::vec3 t = points.A;
			points.A = points.B;
			points.B = t;

			points.Normal = -points.Normal;

			return points;
		}
		virtual CollisionPoints TestCollision(const Transform* transform, const PlaneCollider* collider, const Transform* colliderTransform) const override
		{
			return { }; // No plane-plane collision
		}
	};
}