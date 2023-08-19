#include <Physics/CollisionWorld.h>

namespace Banana::Physics
{
	void CollisionWorld::AddObject(Ref<Object> obj)
	{
		m_Objects.push_back(obj);
	}

	void CollisionWorld::RemoveObject(Ref<Object> obj)
	{
		auto it = std::find(m_Objects.begin(), m_Objects.end(), obj);
		if (it != m_Objects.end())
			m_Objects.erase(it);
	}

	void CollisionWorld::AddSolver(Ref<Solver> solver)
	{
		m_Solvers.push_back(solver);
	}

	void CollisionWorld::RemoveSolver(Ref<Solver> solver)
	{
		auto it = std::find(m_Solvers.begin(), m_Solvers.end(), solver);
		if (it != m_Solvers.end())
			m_Solvers.erase(it);
	}

	void CollisionWorld::SolveCollisions(const std::vector<Collision>& collisions, float timestep)
	{
		for (Ref<Solver> solver : m_Solvers)
			solver->Solve(collisions, timestep);
	}

	void CollisionWorld::SendCollisionCallback(const std::vector<Collision>& collisions, float timestep)
	{
		for (auto& collision : collisions)
		{
			if (m_OnCollision)
				m_OnCollision(collision, timestep);

			auto onCollisionA = collision.ObjA->GetOnCollisionCallback();
			auto onCollisionB = collision.ObjB->GetOnCollisionCallback();

			if (onCollisionA)
				onCollisionA(collision, timestep);
			if (onCollisionB)
				onCollisionB(collision, timestep);
		}
	}

	void CollisionWorld::ResolveCollisions(float timestep)
	{
		std::vector<Collision> collisions;
		std::vector<Collision> triggers;

		for (Ref<Object> a : m_Objects)
		{
			for (Ref<Object> b : m_Objects)
			{
				if (a == b)
					break;
				if (!a->GetCollider() || !b->GetCollider())
					continue;

				CollisionPoints points = a->GetCollider()->TestCollision(a->GetTransform(), b->GetCollider(), b->GetTransform());

				if (!points.HasCollision)
					continue;

				if (a->IsTrigger() || b->IsTrigger())
					triggers.emplace_back(a, b, points);
				else
					collisions.emplace_back(a, b, points);
			}
		}

		SolveCollisions(collisions, timestep);

		SendCollisionCallback(triggers, timestep);
		SendCollisionCallback(collisions, timestep);
	}
}