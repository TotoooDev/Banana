#include <Physics/Solvers/PositionSolver.h>
#include <Physics/Object.h>
#include <Physics/RigidBody.h>

namespace Banana::Physics
{
	void PositionSolver::Solve(const std::vector<Collision>& collisions, float timestep)
	{
		std::vector<std::pair<glm::vec3, glm::vec3>> deltas;

		for (auto& collision : collisions)
		{
			RigidBody* a = collision.ObjA->IsDynamic() ? (RigidBody*)collision.ObjA.get() : nullptr;
			RigidBody* b = collision.ObjB->IsDynamic() ? (RigidBody*)collision.ObjB.get() : nullptr;

			float aInverseMass = a ? 1.0f / a->GetMass() : 0.0f;
			float bInverseMass = b ? 1.0f / b->GetMass() : 0.0f;

			float percent = 0.8f;
			float slop = 0.01f;

			glm::vec3 correction = collision.Points.Normal * 0.8f * glm::max(collision.Points.Depth - 0.01f, 0.0f) * collision.Points.Depth / (aInverseMass + bInverseMass);

			glm::vec3 deltaA = aInverseMass * correction;
			glm::vec3 deltaB = bInverseMass * correction;

			deltas.emplace_back(deltaA, deltaB);
		}

		for (unsigned int i = 0; i < deltas.size(); i++)
		{
			Ref<Object> a = collisions[i].ObjA;
			Ref<Object> b = collisions[i].ObjB;

			a->GetTransform()->Position -= deltas[i].first;
			b->GetTransform()->Position += deltas[i].second;
		}
	}
}