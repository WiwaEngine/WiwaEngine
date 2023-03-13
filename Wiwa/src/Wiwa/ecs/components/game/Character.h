#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Character {
		int healthPoints;
		int damage;
		float speed;
		int shieldRegeneration;
		int range;
		int rof;
	};
}

REFLECTION_BEGIN(Wiwa::Character)
REFLECT_MEMBER(healthPoints)
REFLECT_MEMBER(damage)
REFLECT_MEMBER(speed)
REFLECT_MEMBER(shieldRegeneration)
REFLECT_MEMBER(range)
REFLECT_MEMBER(rof)
REFLECTION_END;