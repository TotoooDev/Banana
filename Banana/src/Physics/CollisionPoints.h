#pragma once
#include <glm/glm.hpp>

namespace Banana::Physics
{
	struct CollisionPoints
	{
		glm::vec3 A; // Furthest point of A into B
		glm::vec3 B; // Furthest point of B into A
		glm::vec3 Normal; // B - A normalized
		float Depth; // Lentgh of B - A
		bool HasCollision;
	};
}