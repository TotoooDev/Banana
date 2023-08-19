#include <Core/Log.h>
#include <Physics/Algo.h>
#include <Physics/Colliders/SphereCollider.h>
#include <Physics/Colliders/PlaneCollider.h>

namespace Banana::Physics::Algo
{
	CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider* a, const Transform* transformA, const SphereCollider* b, const Transform* transformB)
	{
		CollisionPoints points;
		points.HasCollision = false;

		glm::vec3 centerA = a->Center - transformA->Position;
		glm::vec3 centerB = b->Center - transformB->Position;

		// No collision
		if (glm::distance(centerA, centerB) > a->Radius + b->Radius)
			return points;

		auto A = glm::normalize(centerB - centerA) * a->Radius;
		auto B = glm::normalize(centerA - centerB) * b->Radius;
		auto normal = glm::normalize(B - A);
		auto depth = glm::length(B - A);

		points.HasCollision = true;
		points.A = glm::normalize(centerB - centerA) * a->Radius;
		points.B = glm::normalize(centerA - centerB) * b->Radius;
		points.Normal = glm::normalize(points.B - points.A);
		points.Depth = glm::length(points.B - points.A);

		return points;
	}

	CollisionPoints FindSpherePlaneCollisionPoints(const SphereCollider* a, const Transform* transformA, const PlaneCollider* b, const Transform* transformB)
	{
		return {};
	}
}