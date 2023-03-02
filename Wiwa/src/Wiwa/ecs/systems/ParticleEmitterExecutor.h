#pragma once
#include "System.h"
//#include "../components/Transform3D.h"
//#include "../components/AudioSource.h"

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


		void setRoation(glm::vec3 rot, glm::vec3 pos, glm::vec3 up);

		float		lifetime = 0;
		glm::vec4	color;
		glm::vec3	originPosition;
		glm::vec3	localPosition;
		glm::vec3	velocity;
		glm::vec3	acceleration;
		glm::vec3	direction;
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

		void ScreenAlign();

	private:

		void DeleteParticleSystem();

		void AddParticles();

		void UpdateParticles();

		float timer = 0;

		ParticleBillboard* setRotation(const glm::vec3 rot);

		std::vector<std::shared_ptr<ParticleBillboard>> activeParticles;

		//quad_vertices[0] = float3(-1, 1, 0);			quad_vertices[3] = float3(1, 1, 0);


		//quad_vertices[1] = float3(-1, -1, 0);		quad_vertices[2] = float3(1, -1, 0);

		glm::vec3 ref_vertices[4] = 
		{
		glm::vec3(-1, 1, 0),
		glm::vec3(-1, -1, 0),
		glm::vec3(1, 1, 0),
		glm::vec3(1, -1, 0)
		};

		//vertices[0].TexCoords = vec2(0, 1);		vertices[3].TexCoords = vec2(1, 1);


		//vertices[1].TexCoords = vec2(0, 0);			vertices[2].TexCoords = vec2(1, 0);

		glm::vec2 ref_tex_coords[4] = 
		{
			glm::vec2(0, 1),
			glm::vec2(0, 0),
			glm::vec2(1, 1),
			glm::vec2(1, 0)
		};

		//1st triangle
		//indices[0] = 0;


		//indices[1] = 1;		indices[2] = 2;



		//2nd triangle
		//indices[3] = 0;		indices[5] = 3;


		//						indices[4] = 2;

		int ref_vertex_indices[6] =
		{
			0,1,3,
			0,3,2
		};
		
	};
}

REGISTER_SYSTEM(Wiwa::ParticleEmitterExecutor);