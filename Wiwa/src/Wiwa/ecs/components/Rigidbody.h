#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>



namespace Wiwa {
	struct WI_API Rigidbody {
		glm::vec3 positionOffset;
		glm::vec3 scalingOffset;
		float mass;
		float gravity;
		bool isTrigger;
		bool isSensor;
	};
}

REFLECTION_BEGIN(Wiwa::Rigidbody)
	REFLECT_MEMBER(positionOffset)
	REFLECT_MEMBER(scalingOffset)
	REFLECT_MEMBER(mass)
	REFLECT_MEMBER(gravity)
	REFLECT_MEMBER(isTrigger)
	REFLECT_MEMBER(isSensor)
REFLECTION_END;