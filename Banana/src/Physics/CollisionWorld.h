#pragma once
#include <Physics/Object.h>
#include <Physics/Solver.h>
#include <vector>

namespace Banana::Physics
{
	class CollisionWorld
	{
	public:
		void AddObject(Ref<Object> obj);
		void RemoveObject(Ref<Object> obj);

		void AddSolver(Ref<Solver> solver);
		void RemoveSolver(Ref<Solver> solver);

		void SetCollisionCallback(std::function<void(const Collision&, float)> callback) { m_OnCollision = callback; }

		void SolveCollisions(const std::vector<Collision>& collision, float timestep);
		void SendCollisionCallback(const std::vector<Collision>& collisions, float timestep);
		void ResolveCollisions(float timestep);

	protected:
		std::vector<Ref<Object>> m_Objects;
		std::vector<Ref<Solver>> m_Solvers;

		std::function<void(const Collision&, float)> m_OnCollision;
	};
}