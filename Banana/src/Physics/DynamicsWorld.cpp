#include <Physics/DynamicsWorld.h>

namespace Banana::Physics
{
	void DynamicsWorld::AddRigidBody(Ref<RigidBody> body)
	{
		if (body->TakesGravity())
			body->SetGravity(m_Gravity);
		AddObject(body);
	}

	void DynamicsWorld::RemoveRigidBody(Ref<RigidBody> body)
	{
		RemoveObject(body);
	}

	void DynamicsWorld::Step(float timestep)
	{
		ApplyGravity();
		ResolveCollisions(timestep);
		MoveObjects(timestep);
	}

	void DynamicsWorld::ApplyGravity()
	{
		for (Ref<Object> obj : m_Objects)
		{
			if (!obj->IsDynamic())
				continue;

			RigidBody* rigidBody = (RigidBody*)obj.get(); // Don't know if this is good practice lol
			rigidBody->ApplyForce(rigidBody->GetGravity() * rigidBody->GetMass());
		}
	}

	void DynamicsWorld::MoveObjects(float timestep)
	{
		for (Ref<Object> obj : m_Objects)
		{
			if (!obj->IsDynamic())
				continue;

			RigidBody* rigidBody = (RigidBody*)obj.get(); // Don't know if this is good practice lol

			glm::vec3 velocity = rigidBody->GetVelocity() + rigidBody->GetForce() / rigidBody->GetMass() * timestep;
			rigidBody->SetVelocity(velocity);

			glm::vec3 pos = rigidBody->GetTransform()->Position + rigidBody->GetVelocity() * timestep;
			rigidBody->SetPosition(pos);

			rigidBody->SetForce(glm::vec3(0.0f));
		}
	}
}