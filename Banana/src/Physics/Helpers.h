#pragma once
#include <glm/glm.hpp>
#include <reactphysics3d/reactphysics3d.h>

namespace Banana
{
	rp3d::Vector3 RP3DVec3(glm::vec3 vec);
	glm::vec3 GLMVec3(rp3d::Vector3 vec);

	glm::mat4 GLMTransform(rp3d::Transform transform);

	// Thanks Wikipdia
	glm::vec3 QuaternionToEuler(rp3d::Quaternion quaternion);
	rp3d::Quaternion EulerToQuaternion(glm::vec3 euler);
}