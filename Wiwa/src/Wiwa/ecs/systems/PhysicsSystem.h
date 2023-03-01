#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/Rigidbody.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct Object;
	class WI_API PhysicsSystem : public System {
	private:

	public:
		PhysicsSystem();
		~PhysicsSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnSystemAdded() override;

		void OnSystemRemoved() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
		void OnCollision(Object* body1, Object* body2) override;
		void OnCollisionLeave(Object* body1, Object* body2) override;

	private:

		void DeleteBody();
	};
}

REGISTER_SYSTEM(Wiwa::PhysicsSystem);