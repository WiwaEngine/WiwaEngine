#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/AudioSource.h"

#include <vector>
#include <map>

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API ParticleSystem : public System {
	private:

	public:
		ParticleSystem();
		~ParticleSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnSystemAdded() override;

		void OnSystemRemoved() override;

		void ScreenAlign();

	private:

		void DeleteParticleSystem();
	};
}

REGISTER_SYSTEM(Wiwa::ParticleSystem);