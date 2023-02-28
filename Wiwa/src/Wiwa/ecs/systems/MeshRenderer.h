#pragma once
#pragma warning(disable : 4267)
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/Mesh.h"
#include "../components/AudioSource.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API MeshRenderer : public System {
	private:

	public:
		MeshRenderer();
		~MeshRenderer();

		void OnUpdate() override;

		void OnComponentAdded(byte* data, const Type* type) override {}

		void OnComponentRemoved(byte* data, const Type* type) override {}
	};
}

REGISTER_SYSTEM(Wiwa::MeshRenderer);