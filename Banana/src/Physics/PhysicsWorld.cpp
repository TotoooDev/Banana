#include <Physics/PhysicsWorld.h>
#include <Physics/RigidBody.h>
#include <Physics/Helpers.h>

namespace Banana
{
	PhysicsWorld::PhysicsWorld(const PhysicsWorldSettings& settings)
	{
		rp3d::PhysicsWorld::WorldSettings rp3dSettings;
		rp3dSettings.gravity = rp3d::Vector3(settings.Gravity.x, settings.Gravity.y, settings.Gravity.z);

		m_Timestep = settings.Timestep;

		m_World = m_PhysicsCommon.createPhysicsWorld(rp3dSettings);
	}

	PhysicsWorld::~PhysicsWorld()
	{
		m_PhysicsCommon.destroyPhysicsWorld(m_World);
	}

	void PhysicsWorld::Step(float timestep)
	{
		m_Accumulator += timestep;
		while (m_Accumulator >= timestep)
		{
			m_World->update(m_Timestep);
			m_Accumulator -= m_Timestep;
		}
	}

	RigidBody PhysicsWorld::CreateRigidBody(RigidBodyType type, glm::vec3 pos, glm::vec3 rotation)
	{
		rp3d::Transform transform(RP3DVec3(pos), EulerToQuaternion(rotation));
		RigidBody body = RigidBody(m_World->createRigidBody(transform), this);
		switch (type)
		{
		case RigidBodyType::Static: body.m_RigidBody->setType(rp3d::BodyType::STATIC); break;
		case RigidBodyType::Kinematic: body.m_RigidBody->setType(rp3d::BodyType::KINEMATIC); break;
		case RigidBodyType::Dynamic: body.m_RigidBody->setType(rp3d::BodyType::DYNAMIC); break;
		}
		return body;
	}

	void PhysicsWorld::DestroyRigidBody(RigidBody rigidBody)
	{
		m_World->destroyRigidBody(rigidBody.m_RigidBody);
	}
}