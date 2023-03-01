#include <wipch.h>
#include <Wiwa/core/Renderer3D.h>

#include "ParticleEmitterExecutor.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/ecs/components/ParticleEmitter.h"
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>
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
		AddParticle();
	}

	void ParticleEmitterExecutor::OnUpdate()
	{
		Renderer3D& r3d = Application::Get().GetRenderer3D();
		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		LightManager& lman = Wiwa::SceneManager::getActiveScene()->GetLightManager();
		ParticleEmitter* emitterComp = GetComponent<ParticleEmitter>();
		Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(emitterComp->materialId);


		for (std::shared_ptr<ParticleBillboard> p : activeParticles)
		{

			for (size_t i = 0; i < 4; i++)
			{
				p->vertices[i] = ref_vertices[i] + p->transform.position;
			}


			GLuint VAO, VBO, EBO;

			//set mesh
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeof(p->vertices), &p->vertices, GL_STATIC_DRAW);


			std::vector<int> indices;
			for (size_t i = 0; i < 6; i++)
			{
				indices.push_back(p->vertex_indices[i]);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

			glBindVertexArray(0);


			size_t cameraCount = man.getCameraSize();
			std::vector<CameraId>& cameras = man.getCameras();

			for (size_t i = 0; i < cameraCount; i++)
			{
				CameraId cam_id = cameras[i];
				Camera* camera = man.getCamera(cam_id);

				r3d.RenderQuad(VAO, indices, p->transform.position, p->transform.rotation, p->transform.scale,
					lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), /*mat,*/ false, camera, true);
			}

			r3d.RenderQuad(VAO, indices, p->transform.position, p->transform.rotation, p->transform.scale,
				lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), /*mat,*/ false, man.editorCamera, true);

			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}
		//WI_CORE_INFO("test");

	}

	void ParticleEmitterExecutor::OnDestroy()
	{

	}

	void ParticleEmitterExecutor::OnSystemAdded() // Called when system added to the editor
	{

	}

	void ParticleEmitterExecutor::AddParticle()
	{
		std::shared_ptr<ParticleBillboard> p = std::make_shared<ParticleBillboard>();
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

	void ParticleEmitterExecutor::ScreenAlign()
	{
		Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		CameraId cameraId;
		cameraId = cm.getActiveCameraId();
		Wiwa::Camera* cam = cm.getCamera(cameraId);
		//cam->setFront(vector);

		for (size_t i = 0; i < activeParticles.size(); i++)
		{
			glm::vec3 normal = (cam->getPosition() - glm::normalize(activeParticles.at(i).get()->transform.position));
			glm::vec3 up = cam->getUp();
			glm::vec3 right = glm::cross(normal, up);

			glm::mat4 m4(1.0f); //Constructs the Identity Matrix
			m4[3] = (glm::vec4(-right.x, -right.y, -right.z, 1.0f), glm::vec4(up.x, up.y, up.z, 1.0f), glm::vec4(normal.x, normal.y, normal.z, 1.0f));

			//particleManager.setRotation(m4[3]);

			//activeParticles.at(i).get()->setRoation((m4[3]), activeParticles.at(i).get()->transform.position, up);

			activeParticles.at(i).get()->transform.localMatrix = activeParticles.at(i).get()->transform.localMatrix * m4;
		}
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
}


