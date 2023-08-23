#include <Physics/Collider.h>

namespace Banana
{
	Collider::Collider(rp3d::Collider* collider)
		: m_Collider(collider)
	{}

	void Collider::SetBounciness(float bounciness)
	{
		rp3d::Material& material = m_Collider->getMaterial();
		material.setBounciness(bounciness);
	}

	void Collider::SetFriction(float friction)
	{
		rp3d::Material& material = m_Collider->getMaterial();
		material.setFrictionCoefficient(friction);
	}

	void Collider::SetMassDensity(float massDensity)
	{
		rp3d::Material& material = m_Collider->getMaterial();
		material.setMassDensity(massDensity);
	}

	void Collider::SetIsTrigger(bool isTrigger)
	{
		m_Collider->setIsTrigger(isTrigger);
	}
}