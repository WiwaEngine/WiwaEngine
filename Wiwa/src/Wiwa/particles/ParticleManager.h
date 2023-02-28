#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include "Wiwa/ecs/components/ParticleComponent.h"
#include "Wiwa/ecs/components/Billboard.h"
#include "Wiwa/ecs/components/ParticleEmitter.h"

#include <vector>
#include <string>

class Camera;
namespace Wiwa {

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

		struct Emitter {

			Emitter(Billboard& billboard_, const size_t id_) : Billboard_(&billboard_), id(id_) {};

			Billboard* Billboard_;
			size_t id;
		};
	private:
		// Private constructor = default

		// Set last error from result
		static void setLastError(int result);

		// =========== Variables ===========

		static std::list<ParticleManager> Particles;
		std::list<Emitter*> emmiters;
		glm::vec3 billboardRotation;

	public:

		ParticleManager();
		// Init particle engine
		static bool Init();

		// Update audio engine events
		static bool Update();

		// Terminate particle engine
		static bool Terminate();

		//Destroys the Particles
		static bool DestroyAllParticles();

		Emitter* FindByEntityId(size_t id);

		Emitter* setRotation(const glm::vec3 rot);

		bool DeleteEmitter(Emitter* emitter);

		//To create the particles system
		static bool CreateParticle(); //Add Component Particles inside

		static bool SpawnParticle();

		static bool UpdateParticle(); //Add Component Particles inside

		static bool DestroyParticle(); //Add Component Particles inside

		static bool CreateCustomParticle(Wiwa::ParticleComponent* particleComponent);

		// ---------- Billboard ------------

		static bool AddBillboard(Wiwa::Billboard* component);

		static bool UpdateBillBoard(Wiwa::Billboard* component); //Add component Billboard inside

		static bool DrawBillboard(Wiwa::Billboard* component);

		// ---------- Emmitter ------------
		static bool CreatEmitter();

		static bool UpdateEmiiter();

		static bool DestroyEmitter();

		static const uint32_t INVALID_ID = -1;
	};

}
