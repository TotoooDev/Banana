#pragma once
#include <Physics/Collider.h>
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>

namespace Banana
{
	class PhysicsWorld;

	enum class RigidBodyType
	{
		Static, Kinematic, Dynamic
	};

	class RigidBody
	{
	public:
		RigidBody(rp3d::RigidBody* rigidBody, PhysicsWorld* world);

		void InterpolateTransform(float interpolationFactor);

		Collider AddSphereCollider(float radius, glm::vec3 relativePos = glm::vec3(0.0f));
		Collider AddBoxCollider(glm::vec3 halfExtents, glm::vec3 relativePos = glm::vec3(0.0f), glm::vec3 relativeRotation = glm::vec3(0.0f));
		Collider AddCapsuleCollider(float radius, float height, glm::vec3 relativePos = glm::vec3(0.0f), glm::vec3 relativeRotation = glm::vec3(0.0f));
		void SetMass(float mass);
		void SetTakesGravity(bool takesGravity);

		glm::mat4 GetInterpolatedTransform() { return m_InterpolatedTransform; }
		glm::mat4 GetTransform();
		glm::vec3 GetPosition();
		glm::vec3 GetRotation();

	private:
		rp3d::RigidBody* m_RigidBody;
		PhysicsWorld* m_World;

		rp3d::Transform m_PreviousTransform;
		glm::mat4 m_InterpolatedTransform = glm::mat4(1.0f);

		friend class PhysicsWorld;
	};
}