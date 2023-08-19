#pragma once
#include <Core/Ref.h>
#include <Physics/CollisionPoints.h>

namespace Banana::Physics
{
	class Object;

	struct Collision
	{
		Ref<Object> ObjA;
		Ref<Object> ObjB;
		CollisionPoints Points;
	};
}