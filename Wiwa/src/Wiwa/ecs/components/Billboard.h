#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {
	struct WI_API Billboard {
		glm::vec3 billboardAlignement;
		glm::vec3 position;
	};
}

REFLECTION_BEGIN(Wiwa::Billboard)
REFLECT_MEMBER(billboardAlignement)
REFLECT_MEMBER(position)


REFLECTION_END;