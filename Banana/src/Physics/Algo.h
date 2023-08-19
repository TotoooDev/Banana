#pragma once
#include <Physics/Collider.h>

namespace Banana::Physics
{
	struct SphereCollider;
	struct PlaneCollider;

	namespace Algo
	{
		CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider* a, const Transform* transformA, const SphereCollider* b, const Transform* transformB);
		CollisionPoints FindSpherePlaneCollisionPoints(const SphereCollider* a, const Transform* transformA, const PlaneCollider* b, const Transform* transformB);
	}
}