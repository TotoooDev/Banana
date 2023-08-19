#pragma once
#include <Physics/Solver.h>

namespace Banana::Physics
{
	struct PositionSolver : public Solver
	{
		void Solve(const std::vector<Collision>& collisions, float timestep) override;
	};
}