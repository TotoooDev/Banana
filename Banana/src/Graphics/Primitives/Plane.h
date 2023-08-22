#pragma once
#include <Graphics/Mesh.h>

namespace Banana
{
	// From https://winter.dev/prims/algorithms/?s=plane
	class Plane : public Mesh
	{
	public:
		Plane(unsigned int resX, unsigned int resZ);
	};
}