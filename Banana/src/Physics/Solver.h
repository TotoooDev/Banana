#pragma once
#include <Physics/Collision.h>
#include <vector>

namespace Banana::Physics
{
	struct Solver
	{
		virtual void Solve(const std::vector<Collision>& collisions, float timestep) = 0;
	};
}