#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API EnemySpawner {
		int maxEnemiesPerWave;
		int maxWave;
		int currentWave;
		bool hasFinished;
	};
}

REFLECTION_BEGIN(Wiwa::EnemySpawner)
REFLECT_MEMBER(maxEnemiesPerWave)
REFLECT_MEMBER(maxWave)
REFLECT_MEMBER(currentWave)
REFLECT_MEMBER(hasFinished)
REFLECTION_END;