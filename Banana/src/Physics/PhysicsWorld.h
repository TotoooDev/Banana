#pragma once
#include <Physics/RigidBody.h>
#include <glm/glm.hpp>
#include <reactphysics3d/reactphysics3d.h>

namespace Banana
{
	struct PhysicsWorldSettings
	{
		float Timestep = 1.0f / 60.0f;
		glm::vec3 Gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	};

	class PhysicsWorld
	{
	public:
		PhysicsWorld(const PhysicsWorldSettings& settings = PhysicsWorldSettings());
		~PhysicsWorld();

		void Step(float timestep);

		RigidBody CreateRigidBody(RigidBodyType type, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f));
		void DestroyRigidBody(RigidBody rigidBody);

		float GetAccumulator() { return m_Accumulator; }
		float GetTimestep() { return m_Timestep; }

	private:
		rp3d::PhysicsCommon m_PhysicsCommon;
		rp3d::PhysicsWorld* m_World;

		float m_Timestep = 1.0f / 60.0f;
		float m_Accumulator = 0.0f;

		friend class RigidBody;
	};
}