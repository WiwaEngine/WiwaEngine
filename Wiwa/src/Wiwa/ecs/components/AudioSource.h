#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API AudioSource {
		char eventName[32];
		bool playOnAwake;
		bool isPlaying;
	};
}

REFLECTION_BEGIN(Wiwa::AudioSource)
	REFLECT_MEMBER(eventName)
REFLECTION_END;