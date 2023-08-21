#include <Physics/Solvers/ImpulseSolver.h>
#include <Physics/Object.h>
#include <Physics/RigidBody.h>

namespace Banana::Physics
{
	void ImpulseSolver::Solve(const std::vector<Collision>& collisions, float timestep)
	{
		for (auto& collision : collisions)
		{
			RigidBody* a = collision.ObjA->IsDynamic() ? (RigidBody*)collision.ObjA.get() : nullptr;
			RigidBody* b = collision.ObjB->IsDynamic() ? (RigidBody*)collision.ObjB.get() : nullptr;

			glm::vec3 aVelocity = a ? a->GetVelocity() : glm::vec3(0.0f);
			glm::vec3 bVelocity = b ? b->GetVelocity() : glm::vec3(0.0f);
			glm::vec3 relativeVelocity = bVelocity - aVelocity;
			float nSpd = glm::dot(relativeVelocity, collision.Points.Normal);

			float aInverseMass = a ? 1.0f / a->GetMass() : 0.0f;
			float bInverseMass = b ? 1.0f / b->GetMass() : 0.0f;

			// Impulse
			float e = (a ? a->GetRestitution() : 0.5f) * (b ? b->GetRestitution() : 0.5f);
			float j = -(1.0f + e) * nSpd / (aInverseMass + bInverseMass);

			glm::vec3 impulse = j * collision.Points.Normal;

			aVelocity -= impulse * aInverseMass;
			bVelocity += impulse * bInverseMass;

			// Friction
			relativeVelocity = bVelocity - aVelocity;
			nSpd = glm::dot(relativeVelocity, collision.Points.Normal);

			glm::vec3 tangent = relativeVelocity - nSpd * collision.Points.Normal;

			if (glm::length(tangent) > 0.0001f)
				tangent = glm::normalize(tangent);

			float fVelocity = glm::dot(relativeVelocity, tangent);

			float aSF = a ? a->GetStaticFriction() : 0.0f;
			float bSF = b ? b->GetStaticFriction() : 0.0f;
			float aDF = a ? a->GetDynamicFriction() : 0.0f;
			float bDF = b ? b->GetDynamicFriction() : 0.0f;
			float mu = glm::vec2(aSF, bSF).length();

			float f = -fVelocity / (aInverseMass + bInverseMass);

			glm::vec3 friction;
			if (glm::abs(f) < j * mu)
			{
				friction = f * tangent;
			}
			else
			{
				mu = glm::vec2(aDF, bDF).length();
				friction = -j * tangent * mu;
			}

			if (a)
				a->SetVelocity(aVelocity - friction * aInverseMass);
			if (b)
				b->SetVelocity(bVelocity + friction * bInverseMass);
		}
	}
}