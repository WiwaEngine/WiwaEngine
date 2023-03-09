#pragma once

#include "System.h"
#include <vector>
#include <map>

#include <Wiwa/utilities/Reflection.h>

class Camera;
namespace Wiwa {

	struct ParticleBillboard
	{
		glm::vec3 vertices[4];
		glm::vec2 tex_coords[4];
		int vertex_indices[6];
		Transform3D transform;

		float		lifetime = 0;
		glm::vec4	color;

		//position
		glm::vec3	startingPosition;
		glm::vec3	velocity;
		glm::vec3	acceleration;

		//rotation
		glm::vec3	startingRotation;

		//size
		glm::vec3	start_size;
		glm::vec3	growthVelocity;
		glm::vec3	growthAcceleration;


		bool		followEmitter;
		bool		followParticle;

		ParticleBillboard();

	};


	class WI_API ParticleEmitterExecutor : public System {
	private:

	public:
		ParticleEmitterExecutor();
		~ParticleEmitterExecutor();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnSystemAdded() override;

		void OnSystemRemoved() override;

		void ParticleEmitterPath(const char* path);

		void ScreenAlign(std::shared_ptr<ParticleBillboard> particle);

	private:

		void DeleteParticleSystem();

		void AddParticles();

		void UpdateParticles();

		float timer = 0;
		float dt;

		ParticleBillboard* setRotation(const glm::vec3 rot);

		std::vector<std::shared_ptr<ParticleBillboard>> activeParticles;

		glm::vec3 ref_vertices[4] = 
		{
		glm::vec3(-1, 1, 0),
		glm::vec3(-1, -1, 0),
		glm::vec3(1, 1, 0),
		glm::vec3(1, -1, 0)
		};

		glm::vec2 ref_tex_coords[4] = 
		{
			glm::vec2(0, 1),
			glm::vec2(0, 0),
			glm::vec2(1, 1),
			glm::vec2(1, 0)
		};

		int ref_vertex_indices[6] =
		{
			0,1,3,
			0,3,2
		};
	};
}

REGISTER_SYSTEM(Wiwa::ParticleEmitterExecutor);