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

	void ParticleEmitterExecutor::OnDestroy()
	{

	}

	void ParticleEmitterExecutor::UpdateParticles()
	{

		//WI_CORE_INFO("test");


		Renderer3D& r3d = Application::Get().GetRenderer3D();
		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		LightManager& lman = Wiwa::SceneManager::getActiveScene()->GetLightManager();

		ParticleEmitter* emitter = GetComponent<ParticleEmitter>();
		//Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(emitterComp->materialId);


		std::string message = "timer: " + std::to_string(timer);
		WI_CORE_INFO(message.c_str());


		for (size_t j = 0; j < activeParticles.size(); j++)
		{
			std::shared_ptr<ParticleBillboard> p = activeParticles[j];

			//particle death
			if (p->lifetime < 0)
			{
				activeParticles.erase(activeParticles.begin() + j);

				continue;
			}

			p->lifetime -= dt;


			
			//update particles mesh points
			p->velocity += p->acceleration * dt;
			glm::vec3 resultantPosition = p->velocity * dt;

			p->transform.localPosition += resultantPosition;

			for (size_t i = 0; i < 4; i++)
			{
				//update particle variables
				


				p->vertices[i] = ref_vertices[i] + p->originPosition + p->transform.localPosition;

				if (p->followEmitter)
				{
					p->vertices[i] = ref_vertices[i] + p->originPosition + p->transform.localPosition;//+ entity position
				}

				//emitterComp->position
			}

			//ScreenAlign(p);
				
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

					r3d.RenderQuad(VAO, indices, p->transform.position, p->transform.rotation, p->transform.scale,
						lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), /*mat,*/ false, camera, true);
				}

				//show in editor window
				r3d.RenderQuad(VAO, indices, p->transform.position, p->transform.rotation, p->transform.scale,
					lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), /*mat,*/ false, man.editorCamera, true);

				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
				glDeleteBuffers(1, &EBO);
			}
		}
		////WI_CORE_INFO("test");
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

			//set origin position
			if (emitter->particle_originPosition_isRanged)
			{
				float x = Wiwa::Math::RandomRange(emitter->particle_originPosition_range[0].x, emitter->particle_originPosition_range[1].x);
				float y = Wiwa::Math::RandomRange(emitter->particle_originPosition_range[0].y, emitter->particle_originPosition_range[1].y);
				float z = Wiwa::Math::RandomRange(emitter->particle_originPosition_range[0].z, emitter->particle_originPosition_range[1].z);

				p->originPosition = glm::vec3(x, y, z);
			}
			else
			{
				p->originPosition = emitter->particle_originPosition;

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

			//set initial direction
			if (emitter->particle_direction_isRanged)
			{
				float x = Wiwa::Math::RandomRange(emitter->particle_direction_range[0].x, emitter->particle_direction_range[1].x);
				float y = Wiwa::Math::RandomRange(emitter->particle_direction_range[0].y, emitter->particle_direction_range[1].y);
				float z = Wiwa::Math::RandomRange(emitter->particle_direction_range[0].z, emitter->particle_direction_range[1].z);

				p->direction = glm::vec3(x, y, z);
			}
			else
			{
				p->direction = emitter->particle_direction;
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
		glm::vec3 offset;

		glm::vec3 particleDirection = cam->getPosition() - particle->transform.localPosition;
		particleDirection = glm::normalize(particleDirection);
		glm::vec3 right = glm::cross(particleDirection, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 up = glm::cross(right, particleDirection);

		glm::mat4 rotationMatrix = glm::mat4(
			right.x, up.x, particleDirection.x, 0.0f,
			right.y, up.y, particleDirection.y, 0.0f,
			right.z, up.z, particleDirection.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		////Si no funciona almacenar los valores en vez de localMatrix en una m4 temporal
		////Otra solucion es igualar la localMatrix = rotationMatrix
		//particle->transform.localMatrix =  rotationMatrix;

		////glm::vec3 skew;
		////glm::vec4 perspective;
		////glm::vec3 scaling;
		////glm::quat rotation;
		////glm::vec3 translation;
		//////Falla al hacer decompose de la matriz
		//////Valores no inicializados? (transform.position)
		////glm::decompose(particle->transform.localMatrix, scaling, rotation, translation, skew, perspective);
		////particle->transform.position = translation;
		////glm::vec3 eulerAnglesRadians = glm::eulerAngles(rotation);
		////particle->transform.rotation = glm::degrees(eulerAnglesRadians);

		//particle->transform.localPosition = glm::vec3(particle->transform.localMatrix[3]);

		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//Aqui
		glm::mat4 localRotationMatrix = eulerAngleYXZ(particle->transform.localRotation.y, particle->transform.localRotation.x, particle->transform.localRotation.z);
		localRotationMatrix *= rotationMatrix;
		glm::vec3 newLocalRotation = extractEulerAngleYXZ(localRotationMatrix);
		particle->transform.localPosition += offset;
		particle->transform.localMatrix = glm::translate(glm::mat4(1.0f), particle->transform.localPosition) * eulerAngleYXZ(newLocalRotation.y, newLocalRotation.x, newLocalRotation.z);
	}

	void ParticleBillboard::setRoation(glm::vec3 rot, glm::vec3 pos, glm::vec3 up)
	{
		//When particles start drawing in the screen we might need to adjust the formula
		//billboardRotation = rot;

		glm::vec3 direction;
		direction.x = cos(glm::radians(rot.x)) * cos(glm::radians(rot.y));
		direction.y = sin(glm::radians(rot.y));
		direction.z = sin(glm::radians(rot.x)) * cos(glm::radians(rot.y));

		glm::vec3 front = glm::normalize(direction);

		//setFront({ front.x, front.y, front.z });

		//glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);

		glm::lookAt(pos, pos + front, up);
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
		originPosition = glm::vec3(0, 0, 0);
		localPosition = glm::vec3(0, 0, 0);
		velocity = glm::vec3(0, 0, 0);
		acceleration = glm::vec3(0, 0, 0);
		direction = glm::vec3(0, 0, 0);
		followEmitter = false;
		followParticle = false;
		transform.position = glm::vec3(0, 0, 0);
		transform.localPosition = glm::vec3(0, 0, 0);
		transform.rotation = glm::vec3(0, 0, 0);
		transform.localRotation = glm::vec3(0, 0, 0);
		transform.scale = glm::vec3(1, 1, 1);
		transform.localScale = glm::vec3(1, 1, 1);

		//transform.localScale = scale;

	}
}


