#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
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

	private:
		// Private constructor = default

		// Set last error from result
		static void setLastError(int result);

		// =========== Variables ===========

	public:

		ParticleManager();
		// Init particle engine
		static bool Init();

		// Update particle events
		static bool Update();

		// Terminate particle engine
		static bool Terminate();

		//To create a custom particle system
		static bool CreateCustomParticle();

		static const uint32_t INVALID_ID = -1;
	};

}
