#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include "Wiwa/ecs/components/ParticleComponent.h"
#include "Wiwa/ecs/components/Billboard.h"

#include <vector>
#include <string>

class WI_API ParticleManager
{
public:
	struct EventData {
		std::string name;
		uint32_t hash;
	};

	struct BankData : public EventData {
		std::string path;
	};
private:
	// Private constructor = default
	ParticleManager();

	// Set last error from result
	static void setLastError(int result);

	// =========== Variables ===========

	static std::list<ParticleManager> Particles;
	static std::list<ParticleManager> Emmitters;

public:
	// Init particle engine
	static bool Init();

	// Update audio engine events
	static bool Update();

	// Terminate particle engine
	static bool Terminate();

	//Destroys the Particles
	static bool DestroyAllParticles();

	//To create the particles system
	static bool CreateParticle(); //Add Component Particles inside

	static bool SpawnParticle();

	static bool UpdateParticle(); //Add Component Particles inside

	static bool DestroyParticle(); //Add Component Particles inside

	static bool CreateCustomParticle(Wiwa::ParticleComponent* particleComponent);

	// ---------- Billboard ------------

	static bool UpdateBillBoard(Wiwa::Billboard* component); //Add component Billboard inside

	static bool DrawBillboard(Wiwa::Billboard* component);

	// ---------- Emmitter ------------
	static bool CreatEmitter();

	static bool UpdateEmiiter();

	static bool DestroyEmitter();

	static const uint32_t INVALID_ID = -1;
};