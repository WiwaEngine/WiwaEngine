#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Enemy {
		int enemyType;
	};
}

REFLECTION_BEGIN(Wiwa::Enemy)
REFLECT_MEMBER(enemyType)
REFLECTION_END;