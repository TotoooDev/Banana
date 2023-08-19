#pragma once
#include <Physics/Transform.h>
#include <Physics/CollisionPoints.h>

namespace Banana::Physics
{
	struct SphereCollider;
	struct PlaneCollider;

	struct Collider
	{
		virtual CollisionPoints TestCollision(const Transform* transform, const Collider* collider, const Transform* colliderTransform) const = 0;
		virtual CollisionPoints TestCollision(const Transform* transform, const SphereCollider* collider, const Transform* colliderTransform) const = 0;
		virtual CollisionPoints TestCollision(const Transform* transform, const PlaneCollider* collider, const Transform* colliderTransform) const = 0;
	};
}