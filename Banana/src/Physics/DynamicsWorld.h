#pragma once
#include <Physics/CollisionWorld.h>
#include <Physics/RigidBody.h>

namespace Banana::Physics
{
	class DynamicsWorld : public CollisionWorld
	{
	public:
		void AddRigidBody(Ref<RigidBody> body);
		void RemoveRigidBody(Ref<RigidBody> body);

		void SetGravity(glm::vec3 gravity) { m_Gravity = gravity; }

		void Step(float timestep);

	private:
		void ApplyGravity();
		void MoveObjects(float timestep);

	private:
		glm::vec3 m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	};
}