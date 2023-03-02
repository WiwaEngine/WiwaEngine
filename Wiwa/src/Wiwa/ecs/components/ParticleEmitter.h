#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
//#include <Wiwa/utilities/math/Vector3f.h>
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {


	struct WI_API ParticleEmitter {

		ResourceId materialId;

		//Emiter parameters
		float	particle_rate;
		bool	particle_rate_isRanged;
		float	particle_rate_range[2];

		bool	repeat = true;

		int		particle_amount;
		bool	particle_amount_isRanged;
		int		particle_amount_range[2];

		float	particle_lifetime;
		bool	particle_lifetime_isRanged;
		float	particle_lifetime_range[2];

		//std::list<ColorTime> particle_color;

		int nextPos = 100;
		glm::vec4 nextColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glm::vec3	particle_originPosition;
		bool		particle_originPosition_isRanged;
		glm::vec3	particle_originPosition_range[2];

		glm::vec3	particle_velocity;
		bool		particle_velocity_isRanged;
		glm::vec3	particle_velocity_range[2];

		glm::vec3	particle_acceleration;
		bool		particle_acceleration_isRanged;
		glm::vec3	particle_acceleration_range[2];

		glm::vec3	particle_direction;
		bool		particle_direction_isRanged;
		glm::vec3	particle_direction_range[2];

		bool		particle_followEmitter = false;
		bool		particle_followParticle = false;;


	};
}

REFLECTION_BEGIN(Wiwa::ParticleEmitter)

	//REFLECT_MEMBER(Position)
	//REFLECT_MEMBER(Color)
	//REFLECT_MEMBER(lifeTime)
	//REFLECT_MEMBER(speed)
	//REFLECT_MEMBER(size)
	//REFLECT_MEMBER(directionVariation)
	//REFLECT_MEMBER(distanceToCamera)

REFLECTION_END;