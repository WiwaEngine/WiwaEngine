#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {
	struct WI_API ParticleComponent {
		glm::vec3 Position;
		glm::vec3 Color;
		float lifeTime;
		float speed;
		float size;
		float directionVariation;
		float distanceToCamera;
	};
}

REFLECTION_BEGIN(Wiwa::ParticleComponent)
REFLECT_MEMBER(Position)
REFLECT_MEMBER(Color)
REFLECT_MEMBER(lifeTime)
REFLECT_MEMBER(speed)
REFLECT_MEMBER(size)
REFLECT_MEMBER(directionVariation)
REFLECT_MEMBER(distanceToCamera)

REFLECTION_END;