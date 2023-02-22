#include <wipch.h>
#include "ParticleSystem.h"
#include "Wiwa/core/Resources.h"
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
		Wiwa::ParticleComponent* componentParticle = GetComponent<Wiwa::ParticleComponent>();

		ParticleManager& particleManager = m_Scene->GetParticleManager();

		// Create object particles from pure data
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
			particleManager.DeleteEmitter(emitter);
	}
}


