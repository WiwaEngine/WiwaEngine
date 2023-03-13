#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Wave {
		int maxEnemies;
		int currentEnemiesAlive;
		bool hasFinished;
	};
}

REFLECTION_BEGIN(Wiwa::Wave)
REFLECT_MEMBER(maxEnemies)
REFLECT_MEMBER(currentEnemiesAlive)
REFLECT_MEMBER(hasFinished)
REFLECTION_END;