#pragma once
#include <reactphysics3d/reactphysics3d.h>

namespace Banana
{
	class Collider
	{
	public:
		Collider(rp3d::Collider* collider);

		void SetBounciness(float bounciness);
		void SetFriction(float friction);
		void SetMassDensity(float massDensity);
		void SetIsTrigger(bool isTrigger);

	private:
		rp3d::Collider* m_Collider;
	};
}