#include <wipch.h>
#include "ParticleSystem.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/utilities/render/Model.h"
#include "Wiwa/ecs/components/Mesh.h"
#include "Wiwa/particles/ParticleManager.h"
#include "Wiwa/ecs/components/ParticleComponent.h"
#include "Wiwa/ecs/components/Billboard.h"

namespace Wiwa {
	ParticleSystem::ParticleSystem()
	{

	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::OnAwake()
	{
		
	}

	void ParticleSystem::OnInit()
	{
		
	}

	void ParticleSystem::OnUpdate()
	{
		Wiwa::Billboard* billboardComp = GetComponent<Wiwa::Billboard>();

		ParticleManager& particleManager = m_Scene->GetParticleManager();
		particleManager.UpdateBillBoard(billboardComp);
		
	}

	void ParticleSystem::OnDestroy()
	{
		DeleteParticleSystem();
	}

	void ParticleSystem::OnSystemAdded() // Called when system added to the editor
	{
		/*
		Wiwa::ParticleComponent* componentParticle = GetComponent<Wiwa::ParticleComponent>();

		ParticleManager& particleManager = m_Scene->GetParticleManager();

		// Create object particles from pure data

		//----------------------------------------------------------------------

		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Init physics of --> {}", e_name);

		Rigidbody* rb = GetComponent<Rigidbody>();
		Transform3D* transform = GetComponent<Transform3D>();

		ColliderCube* cube = GetComponent<ColliderCube>();
		ColliderSphere* sphere = GetComponent<ColliderSphere>();
		ColliderCylinder* cylinder = GetComponent<ColliderCylinder>();

		Mesh* mesh = GetComponent<Mesh>();

		PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		if (rb)
		{
			if (cube)
			{
				if (mesh) cube->halfExtents = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId)->boundingBox.HalfSize();
				physicsManager.AddBodyCube(m_EntityId, *cube, *transform, *rb);
			}
			else if (sphere)
			{
				physicsManager.AddBodySphere(m_EntityId, *sphere, *transform, *rb);
			}
			else if (cylinder)
			{
				physicsManager.AddBodyCylinder(m_EntityId, *cylinder, *transform, *rb);
			}
		}
		//------------------------------------------------------------------------------

		Particles* newParticle = new Particles(particleReference);

		if (emitterInstance->owner->owner->GetComponent(COMPONENT_TYPES::BILLBOARD) == false)
		{
			newParticle->billboard = (ModuleComponentBillBoard*)emitterInstance->owner->owner->CreateComponent(COMPONENT_TYPES::BILLBOARD);
		}
		else
		{
			newParticle->billboard = (ModuleComponentBillBoard*)emitterInstance->owner->owner->GetComponent(COMPONENT_TYPES::BILLBOARD);
		}

		if (newParticle != nullptr)
		{
			particles_vector.push_back(newParticle);
			existing_particles++;
			activeParticles++;
			particles_vector[particles_vector.size() - 1].direction = particleReference->direction + SetRandomDirection();
		}
		*/

		//------------------------------------------------------------------------------
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		//const char* e_name = entityManager.GetEntityName(m_EntityId);
		//WI_INFO("Init particles: {}", e_name);

		ParticleManager& particleManager = m_Scene->GetParticleManager();

		Billboard* billboard = GetComponent<Billboard>();
		Transform3D* transform = GetComponent<Transform3D>();
		ParticleComponent* particleComponent = GetComponent<ParticleComponent>();
		Mesh* mesh = GetComponent<Mesh>();
		ParticleManager::Emitter* emitter = particleManager.FindByEntityId(m_EntityId);

		ParticleComponent* newParticle = new ParticleComponent();

		if (emitter->Billboard_ == false)
		{
			// particleManager.CreateParticle();	Here to add the component Billboard

			particleManager.AddBillboard(emitter->Billboard_);
		}

		else
		{
			emitter->Billboard_ = GetComponent<Billboard>();
		}
	}

	void ParticleSystem::ScreenAlign()
	{
		/*
		switch (billboard_alignment)
		{
		case Billboarding_Alignment::SCREEN_ALIGNED:
			ScreenAlign();
			break;
		case Billboarding_Alignment::WORLD_ALIGNED:
			WorldAlign();
			break;
		case Billboarding_Alignment::AXIS_ALIGNED:
			AxisAlign();
			break;
		default:
			break;
		}
		*/
		//-----------------------------------------------------------------------------------------------
		/*
		float3 normal = (App->camera->GetPosition() - this->transform->GetPosition()).Normalized();
		float3 up = App->camera->GetCurrentCamera()->GetFrustum().Up();
		float3 right = normal.Cross(up);

		float3x3 mat = float3x3::identity;
		mat.Set(-right.x, -right.y, -right.z, up.x, up.y, up.z, normal.x, normal.y, normal.z);

		transform->SetRotation(mat.Inverted().ToEulerXYZ());
		*/
		//-----------------------------------------------------------------------------------------------

		//The main goal here is to apply the alignment to the billboard of the particle
		//(Moving the plain in the direction of the camera)

		Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		CameraId cameraId;
		cameraId = cm.getActiveCameraId();
		Wiwa::Camera* cam = cm.getCamera(cameraId);
		//cam->setFront(vector);
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		ParticleManager& particleManager = m_Scene->GetParticleManager();

		glm::vec3 normal = (cam->getPosition() -  glm::normalize(particleManager.FindByEntityId(m_EntityId)->Billboard_->position));
		glm::vec3 up = cam->getUp();
		glm::vec3 right = glm::cross(normal, up);

		glm::mat4 m4(1.0f); //Constructs the Identity Matrix
		m4[3] = (glm::vec4(-right.x, -right.y, -right.z, 1.0f), glm::vec4(up.x, up.y, up.z, 1.0f), glm::vec4(normal.x, normal.y, normal.z, 1.0f));

		particleManager.setRotation(m4[3]);
	}

	void ParticleSystem::OnSystemRemoved() // Called when system removed to the editor
	{
		// Remove that previously created object from pure data
		DeleteParticleSystem();
	}

	void ParticleSystem::DeleteParticleSystem()
	{
		//---------------------------------------------------------------------------------
		ParticleManager& particleManager = m_Scene->GetParticleManager();

		ParticleManager::Emitter* emitter = particleManager.FindByEntityId(m_EntityId);

		if (emitter != nullptr)
		{
			particleManager.DeleteEmitter(emitter);
		}
	}
}


