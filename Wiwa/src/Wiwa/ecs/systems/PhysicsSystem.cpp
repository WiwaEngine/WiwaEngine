#include <wipch.h>
#include "PhysicsSystem.h"
#include "Wiwa/physics/PhysicsManager.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/utilities/render/Model.h"
#include "Wiwa/ecs/components/Mesh.h"

namespace Wiwa {
	PhysicsSystem::PhysicsSystem()
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::OnAwake()
	{
	}

	void PhysicsSystem::OnInit()
	{
	}

	void PhysicsSystem::OnUpdate()
	{
		//PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

	/*	Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Updating physics of --> {}", e_name);*/
	}

	void PhysicsSystem::OnDestroy()
	{
		DeleteBody();
	}

	void PhysicsSystem::OnSystemAdded()
	{
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);

		Rigidbody* rb = GetComponent<Rigidbody>();
		Transform3D* transform = GetComponent<Transform3D>();

		ColliderCube* cube = GetComponent<ColliderCube>();
		ColliderSphere* sphere = GetComponent<ColliderSphere>();
		ColliderCapsule* capsule = GetComponent<ColliderCapsule>();

		Mesh* mesh = GetComponent<Mesh>();

		PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		if (rb)
		{
			if (cube)
			{
				std::string name = "City_building_004";
				WI_INFO("NEW BODY OF ENTITY--> {} AND ID {}", e_name, m_EntityId);
				//if (mesh) cube->halfExtents = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId)->boundingBox.HalfSize();
				physicsManager.AddBodyCube(m_EntityId, *cube, *transform, *rb);
				if (name == e_name)
					physicsManager.AddBodyToLog(physicsManager.FindByEntityId(m_EntityId));
			}
			else if (sphere)
			{
				physicsManager.AddBodySphere(m_EntityId, *sphere, *transform, *rb);
			}
			else if (capsule)
			{
				physicsManager.AddBodyCapsule(m_EntityId, *capsule, *transform, *rb);
			}
		}
	}
	void PhysicsSystem::OnSystemRemoved()
	{
		DeleteBody();
	}

	void PhysicsSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(body1->id);
		WI_INFO("Collision enter of --> {}", e_name);
	}

	void PhysicsSystem::OnCollision(Object* body1, Object* body2)
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(body1->id);
		WI_INFO("Collision continuwus of --> {}", e_name);
	}

	void PhysicsSystem::OnCollisionLeave(Object* body1, Object* body2)
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(body1->id);
		WI_INFO("Collision leave of --> {}", e_name);
	}

	void PhysicsSystem::DeleteBody()
	{
		PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		Object* body = physicsManager.FindByEntityId(m_EntityId);

		if (body != nullptr)
			physicsManager.DeleteBody(body);
	}
}