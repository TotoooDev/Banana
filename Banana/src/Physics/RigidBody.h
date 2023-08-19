#pragma once
#include <Physics/Object.h>

namespace Banana::Physics
{
	class RigidBody : public Object
	{
	public:
		RigidBody(Transform* transform, Collider* collider, float mass = 1.0f, bool takesGravity = true)
			: Object(transform, collider, true), m_Mass(mass), m_TakesGravity(takesGravity)
		{}

		glm::vec3 GetGravity() { return m_Gravity; }
		glm::vec3 GetForce() { return m_Force; }
		glm::vec3 GetVelocity() { return m_Velocity; }
		float GetStaticFriction() { return m_StaticFriction; }
		float GetDynamicFriction() { return m_DynamicFriction; }
		float GetRestitution() { return m_Restitution; }
		float GetMass() { return m_Mass; }
		bool TakesGravity() { return m_TakesGravity; }

		void SetGravity(glm::vec3 gravity) { m_Gravity = gravity; }
		void SetForce(glm::vec3 force) { m_Force = force; }
		void ApplyForce(glm::vec3 force) { m_Force += force; }
		void SetVelocity(glm::vec3 velocity) { m_Velocity = velocity; }
		void SetStaticFriction(float sf) { m_StaticFriction = sf; }
		void SetDynamicFriction(float df) { m_DynamicFriction = df; }
		void SetRestitution(float restitution) { m_Restitution = restitution; }
		void SetMass(float mass) { m_Mass = mass; }
		void SetTakesGravity(bool takesGravity) { m_TakesGravity = takesGravity; }

	private:
		glm::vec3 m_Gravity = glm::vec3(0.0f);
		glm::vec3 m_Force = glm::vec3(0.0f);
		glm::vec3 m_Velocity = glm::vec3(0.0f);

		float m_StaticFriction = 1.0f;
		float m_DynamicFriction = 1.0f;
		float m_Restitution = 1.0f;

		float m_Mass;
		bool m_TakesGravity;
	};
}