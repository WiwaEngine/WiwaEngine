#pragma once

#include "System.h"
#include <vector>
#include <map>

#include <Wiwa/utilities/Reflection.h>

class Camera;
namespace Wiwa {

	struct ParticleBillboard
	{
		Material m_material;
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

		void ScreenAlign(std::shared_ptr<ParticleBillboard> particle);

		glm::mat4 setRoation(glm::vec3 rot, glm::vec3 pos, glm::vec3 up);

		glm::mat4 eulerAngleYXZ(float yaw, float pitch, float roll)
		{
			glm::mat3 rotX = glm::mat3(1.0f, 0.0f, 0.0f, 0.0f, cos(pitch), sin(pitch), 0.0f, -sin(pitch), cos(pitch));
			glm::mat3 rotY = glm::mat3(cos(yaw), 0.0f, -sin(yaw), 0.0f, 1.0f, 0.0f, sin(yaw), 0.0f, cos(yaw));
			glm::mat3 rotZ = glm::mat3(cos(roll), sin(roll), 0.0f, -sin(roll), cos(roll), 0.0f, 0.0f, 0.0f, 1.0f);

			return rotY * rotX * rotZ;
		}
		glm::vec3 extractEulerAngleYXZ(glm::mat3 rotationMatrix)
		{
			float pitch = asin(rotationMatrix[1][2]);
			float yaw = atan2(-rotationMatrix[0][2], rotationMatrix[2][2]);
			float roll = atan2(-rotationMatrix[1][0], rotationMatrix[1][1]);

			return glm::vec3(yaw, pitch, roll);
		}

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
		glm::vec3(-1, 1, 0),	//0		0	  2   		1	  3
		glm::vec3(-1, -1, 0),	//1				  		
		glm::vec3(1, 1, 0),		//2				  		
		glm::vec3(1, -1, 0)		//3		1	  3	  		0	  2
		
		};

		glm::vec2 ref_tex_coords[4] = 
		{
			glm::vec2(0, 0),	//0		0	  2  		1	  3
			glm::vec2(0, 1),	//1			     		
			glm::vec2(1, 0),	//2			     		
			glm::vec2(1, 1)		//3		1	  3  		0	  2
							
		};

		int ref_vertex_indices[6] =
		{
			0,1,3,
			0,3,2
		};
	};
}

REGISTER_SYSTEM(Wiwa::ParticleEmitterExecutor);