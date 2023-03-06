#include <wipch.h>
#include <Wiwa/core/Renderer3D.h>
#include "ParticleEmitterExecutor.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/ecs/components/ParticleEmitter.h"
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>
#include <Wiwa/utilities/math/Math.h>
#include <glew.h>

namespace Wiwa {
	ParticleEmitterExecutor::ParticleEmitterExecutor()
	{

	}

	ParticleEmitterExecutor::~ParticleEmitterExecutor()
	{

	}

	void ParticleEmitterExecutor::OnAwake()
	{

	}

	void ParticleEmitterExecutor::OnInit()
	{
		WI_CORE_INFO("Init");

		AddParticles();
	}

	void ParticleEmitterExecutor::OnUpdate()
	{

		ParticleEmitter* emitter = GetComponent<ParticleEmitter>();

		dt = Time::GetRealDeltaTime() / 1000;

		if (emitter)
		{
			if (timer <= 0)
			{
				if (emitter->repeat)
				{
					if (emitter->particle_rate_isRanged)
					{
						timer = Wiwa::Math::RandomRange(emitter->particle_rate_range[0], emitter->particle_rate_range[1]);
					}
					else
					{
						timer = emitter->particle_rate;
					}
				}

				AddParticles();
			}
			else
			{
				timer -= dt;

			}

			UpdateParticles();
		}
	}

	void ParticleEmitterExecutor::OnDestroy()
	{

	}

	void ParticleEmitterExecutor::UpdateParticles()
	{
		Renderer3D& r3d = Application::Get().GetRenderer3D();
		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		LightManager& lman = Wiwa::SceneManager::getActiveScene()->GetLightManager();

		ParticleEmitter* emitter = GetComponent<ParticleEmitter>();
		Transform3D* t3D = GetComponent<Transform3D>();

		//Add material: (currently only a shader)
		//Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(emitterComp->materialId);

		for (size_t j = 0; j < activeParticles.size(); j++)
		{
			std::shared_ptr<ParticleBillboard> p = activeParticles[j];

			//particle death
			if (p->lifetime < 0)
			{
				activeParticles.erase(activeParticles.begin() + j);

				continue;
			}

			//particles look at the camera:
			ScreenAlign(p);

			p->lifetime -= dt;
			
			//update particles mesh points
			
			//position
			p->velocity += p->acceleration * dt;
			glm::vec3 resultantPosition = p->velocity * dt;

			p->transform.position += resultantPosition;

			if (p->startingRotation.x != 0.0f || p->startingRotation.y != 0.0f || p->startingRotation.z != 0.0f)
			{
				p->transform.rotation = p->startingRotation;
			}

			/*if (p->start_size.x == 0.0f && p->start_size.y == 0.0f && p->start_size.z == 0.0f)
			{
				p->start_size.x = 1.0f;
				p->start_size.y = 1.0f;
				p->start_size.z = 1.0f;
			}*/

			//size
			p->transform.localScale += p->growthVelocity * dt;

			for (size_t i = 0; i < 4; i++)
			{
				//update particle variables

				p->vertices[i] = (ref_vertices[i] + p->startingPosition + p->transform.position);

				if (p->followEmitter)
				{
					p->vertices[i] = (ref_vertices[i] + t3D->position + p->startingPosition + p->transform.position);//+ entity position
				}
				//emitterComp->position
			}

			//draw particles
			{

				GLuint VAO, VBO, EBO;

				//set mesh
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);

				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeof(p->vertices), &p->vertices, GL_DYNAMIC_DRAW);



				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p->vertex_indices) * sizeof(int), &p->vertex_indices, GL_DYNAMIC_DRAW);

				// vertex positions
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

				glBindVertexArray(0);


				size_t cameraCount = man.getCameraSize();
				std::vector<CameraId>& cameras = man.getCameras();


				std::vector<int> indices;
				for (size_t i = 0; i < 6; i++)
				{
					indices.push_back(p->vertex_indices[i]);
				}

				//show in game cameras
				for (size_t i = 0; i < cameraCount; i++)
				{
					CameraId cam_id = cameras[i];
					Camera* camera = man.getCamera(cam_id);

					r3d.RenderQuad(VAO, indices, p->transform.position, p->transform.rotation, p->transform.localScale,
						lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), /*mat,*/ false, camera, true);
				}

				//show in editor window
				r3d.RenderQuad(VAO, indices, p->transform.position, p->transform.rotation, p->transform.localScale,
					lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), /*mat,*/ false, man.editorCamera, true);

				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
				glDeleteBuffers(1, &EBO);
			}
		}
	}

	void ParticleEmitterExecutor::OnSystemAdded() // Called when system added to the editor
	{

	}

	void ParticleEmitterExecutor::AddParticles()
	{

		

		Transform3D* transform = GetComponent<Transform3D>();
		ParticleEmitter* emitter = GetComponent<ParticleEmitter>();

		int amountToAdd = emitter->particle_amount;

		if (emitter->particle_amount_isRanged)
		{
			amountToAdd = Wiwa::Math::RandomRange(emitter->particle_amount_range[0], emitter->particle_amount_range[1]);
		}


		for (size_t i = 0; i < amountToAdd; i++)
		{

			std::shared_ptr<ParticleBillboard> p = std::make_shared<ParticleBillboard>();

			//set particle lifetime
			if (emitter->particle_lifetime_isRanged)
			{
				p->lifetime = Wiwa::Math::RandomRange(emitter->particle_lifetime_range[0], emitter->particle_lifetime_range[1]);
			}
			else
			{
				p->lifetime = emitter->particle_lifetime;
			}

			//position
			{
				//set starting position
				if (emitter->particle_startingPosition_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range[0].x, emitter->particle_startingPosition_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range[0].y, emitter->particle_startingPosition_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range[0].z, emitter->particle_startingPosition_range[1].z);

					p->startingPosition = glm::vec3(x, y, z);
				}
				else
				{
					p->startingPosition = emitter->particle_startingPosition;
				}

				//set initial velocity
				if (emitter->particle_velocity_isRanged)
				{

					float x = Wiwa::Math::RandomRange(emitter->particle_velocity_range[0].x, emitter->particle_velocity_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_velocity_range[0].y, emitter->particle_velocity_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_velocity_range[0].z, emitter->particle_velocity_range[1].z);

					p->velocity = glm::vec3(x, y, z);
				}
				else
				{
					p->velocity = emitter->particle_velocity;
				}

				//set initial acceleration
				if (emitter->particle_acceleration_isRanged)
				{

					float x = Wiwa::Math::RandomRange(emitter->particle_acceleration_range[0].x, emitter->particle_acceleration_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_acceleration_range[0].y, emitter->particle_acceleration_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_acceleration_range[0].z, emitter->particle_acceleration_range[1].z);

					p->acceleration = glm::vec3(x, y, z);
				}
				else
				{
					p->acceleration = emitter->particle_acceleration;
				}
			}
			
			//rotation
			{
				//set starting rotation
				if (emitter->particle_startingRotation_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range[0].x, emitter->particle_startingRotation_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range[0].y, emitter->particle_startingRotation_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range[0].z, emitter->particle_startingRotation_range[1].z);

					p->startingRotation = glm::vec3(x, y, z);
				}
				else
				{
					p->startingRotation.x = emitter->particle_startingRotation.x;
					p->startingRotation.y = emitter->particle_startingRotation.y;
					p->startingRotation.z = emitter->particle_startingRotation.z;
				}
			}

			//scale
			{
				glm::vec3 startingSize(1.0f, 1.0f, 1.0f);

				if (emitter->particle_startingSize.x != 0.0f || emitter->particle_startingSize.y != 0.0f || emitter->particle_startingSize.z != 0.0f)
				{
					p->start_size = emitter->particle_startingSize;
				}

				else
				{
					emitter->particle_startingSize = startingSize;
				}

				//set initial size growth rate
				if (emitter->particle_growthVelocity_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range[0].x, emitter->particle_growthVelocity_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range[0].y, emitter->particle_growthVelocity_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range[0].z, emitter->particle_growthVelocity_range[1].z);

					p->growthVelocity = glm::vec3(x, y, z);
				}
				else
				{
					p->growthVelocity = emitter->particle_growthVelocity;
				}

				//set initial size
				if (emitter->particle_growthVelocity_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_startingSize_range[0].x, emitter->particle_startingSize_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingSize_range[0].y, emitter->particle_startingSize_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingSize_range[0].z, emitter->particle_startingSize_range[1].z);

					p->transform.localScale = glm::vec3(x, y, z);
				}
				else
				{
					p->transform.localScale = emitter->particle_startingSize;
				}
			}

			p->followEmitter = emitter->particle_followEmitter;
			p->followParticle = emitter->particle_followParticle;


			//set geometry
			for (size_t i = 0; i < 4; i++)
			{
				p->vertices[i] = ref_vertices[i];
				p->tex_coords[i] = ref_tex_coords[i];
			}
			for (size_t i = 0; i < 6; i++)
			{
				p->vertex_indices[i] = ref_vertex_indices[i];
			}

			p->transform.scale = glm::vec3(1, 1, 1);

			activeParticles.push_back(p);
		}

	}

	void ParticleEmitterExecutor::ScreenAlign(std::shared_ptr<ParticleBillboard> particle)
	{
		Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		CameraId cameraId;
		cameraId = cm.getActiveCameraId();
		Wiwa::Camera* cam = cm.getCamera(cameraId);
		glm::vec3 offset = glm::vec3(-90.0f, -60.0f, 0.0f); //Adjust the desired offset

		particle->transform.rotation.x = cam->getRotation().x + offset.x;
		particle->transform.rotation.y = cam->getRotation().y + offset.y;
		particle->transform.rotation.z = cam->getRotation().z + offset.z;
	}

	void ParticleEmitterExecutor::OnSystemRemoved() // Called when system removed to the editor
	{

	}

	void ParticleEmitterExecutor::DeleteParticleSystem()
	{
		
	}

	ParticleBillboard::ParticleBillboard()
	{
		//set variables
		
		lifetime = 1;
		color = glm::vec4(1, 1, 1, 1);
		
		//position
		transform.position = glm::vec3(0, 0, 0);
		transform.localPosition = glm::vec3(0, 0, 0);

		startingPosition = glm::vec3(0, 0, 0);
		velocity = glm::vec3(0, 0, 0);
		acceleration = glm::vec3(0, 0, 0);


		//rotation
		transform.rotation = glm::vec3(0, 0, 0);
		transform.localRotation = glm::vec3(0, 0, 0);

		startingRotation = glm::vec3(0, 0, 0);


		//scale
		transform.scale = glm::vec3(1, 1, 1);
		transform.localScale = glm::vec3(1, 1, 1);

		start_size = glm::vec3(1.0f, 1.0f, 1.0f);
		growthVelocity = glm::vec3(0, 0, 0);
		growthAcceleration = glm::vec3(0, 0, 0);


		followEmitter = false;
		followParticle = false;
	}
}


