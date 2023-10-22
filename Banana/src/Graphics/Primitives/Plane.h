#pragma once
#include <Graphics/Mesh.h>

namespace Banana
{
	/**
	 * From https://winter.dev/prims/algorithms/?s=plane
	 */
	class Plane : public Mesh
	{
	public:
		/**
		 * Creates a plane shape.
		 * @param resX - The resolution in the X direction.
		 * @param resZ - The resolution in the Z direction.
		 */
		Plane(unsigned int resX, unsigned int resZ);
	};
}