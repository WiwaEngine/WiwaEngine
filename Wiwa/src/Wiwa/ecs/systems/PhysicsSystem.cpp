#include <wipch.h>
#include "PhysicsSystem.h"

namespace Wiwa {
	PhysicsSystem::PhysicsSystem()
	{

	}
	PhysicsSystem::~PhysicsSystem()
	{

	}

	// When you


	void PhysicsSystem::OnUpdate()
	{
		Rigidbody* rb = GetComponent<Rigidbody>();

		if (rb) {
			WI_INFO("{}", rb->x);
		}
	}
}