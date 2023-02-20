#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/Rigidbody.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
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

	private:

		void DeleteBody();
	};
}

REGISTER_SYSTEM(Wiwa::PhysicsSystem);