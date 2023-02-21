#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include "Wiwa/ecs/components/ParticleComponent.h"

#include <vector>
#include <string>

class WI_API Particle
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
	Particle();

	// Set last error from result
	static void setLastError(int result);

	// =========== Variables ===========

	static std::list<Particle> Particles;
	static std::list<Particle> Emmitters;


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

	// ---------- Emmitter ------------
	static bool CreatEmitter();

	static bool UpdateEmiiter();

	static bool DestroyEmitter();

	static const uint32_t INVALID_ID = -1;
};