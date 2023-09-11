#include <Physics/RigidBody.h>
#include <Physics/PhysicsWorld.h>
#include <Physics/Helpers.h>
#include <glm/gtx/matrix_decompose.hpp>

namespace Banana
{
	RigidBody::RigidBody(rp3d::RigidBody* rigidBody, PhysicsWorld* world)
		: m_RigidBody(rigidBody), m_World(world)
	{
		
	}

	void RigidBody::InterpolateTransform(float interpolationFactor)
	{
		rp3d::Transform currentTransform = m_RigidBody->getTransform();
		rp3d::Transform interpolatedTransform = rp3d::Transform::interpolateTransforms(m_PreviousTransform, currentTransform, interpolationFactor);
		m_InterpolatedTransform = GLMTransform(interpolatedTransform);
		m_PreviousTransform = currentTransform;
	}

	Collider RigidBody::AddSphereCollider(float radius, glm::vec3 relativePos)
	{
		rp3d::SphereShape* shape = m_World->m_PhysicsCommon.createSphereShape(radius);
		rp3d::Transform transform(RP3DVec3(relativePos), rp3d::Quaternion());
		rp3d::Collider* collider = m_RigidBody->addCollider(shape, transform);
		return Collider(collider);
	}

	Collider RigidBody::AddBoxCollider(glm::vec3 halfExtents, glm::vec3 relativePos, glm::vec3 relativeRotation)
	{
		rp3d::BoxShape* shape = m_World->m_PhysicsCommon.createBoxShape(RP3DVec3(halfExtents));
		rp3d::Transform transform(RP3DVec3(relativePos), EulerToQuaternion(relativeRotation));
		rp3d::Collider* collider = m_RigidBody->addCollider(shape, transform);
		return Collider(collider);
	}

	Collider RigidBody::AddCapsuleCollider(float radius, float height, glm::vec3 relativePos, glm::vec3 relativeRotation)
	{
		rp3d::CapsuleShape* shape = m_World->m_PhysicsCommon.createCapsuleShape(radius, height);
		rp3d::Transform transform(RP3DVec3(relativePos), EulerToQuaternion(relativeRotation));
		rp3d::Collider* collider = m_RigidBody->addCollider(shape, transform);
		return Collider(collider);
	}

	void RigidBody::SetPosition(glm::vec3 pos)
	{
		rp3d::Transform transform(RP3DVec3(pos), m_RigidBody->getTransform().getOrientation());
		m_RigidBody->setTransform(transform);
	}

	void RigidBody::SetMass(float mass)
	{
		m_RigidBody->setMass(mass);
	}

	void RigidBody::SetTakesGravity(bool takesGravity)
	{
		m_RigidBody->enableGravity(takesGravity);
	}

	void RigidBody::AddVelocity(glm::vec3 velocity)
	{
		rp3d::Vector3 newVelocity = m_RigidBody->getLinearVelocity() + RP3DVec3(velocity);
		m_RigidBody->setLinearVelocity(newVelocity);
	}

	void RigidBody::SetVelocity(glm::vec3 velocity)
	{
		m_RigidBody->setLinearVelocity(RP3DVec3(velocity));
	}

	void RigidBody::SetAngularVelocity(glm::vec3 velocity)
	{
		m_RigidBody->setAngularVelocity(RP3DVec3(velocity));
	}

	void RigidBody::ApplyForce(glm::vec3 force)
	{
		m_RigidBody->applyLocalForceAtCenterOfMass(RP3DVec3(force));
	}

	glm::mat4 RigidBody::GetTransform()
	{
		return GLMTransform(m_RigidBody->getTransform());
	}

	glm::vec3 RigidBody::GetPosition()
	{
		return GLMVec3(m_RigidBody->getTransform().getPosition());
	}

	glm::vec3 RigidBody::GetRotation()
	{
		return QuaternionToEuler(m_RigidBody->getTransform().getOrientation());
	}

	glm::vec3 RigidBody::GetVelocity()
	{
		return GLMVec3(m_RigidBody->getLinearVelocity());
	}
}