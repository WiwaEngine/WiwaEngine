#pragma once
#pragma warning(disable : 4267)
#include "System.h"
#include "../components/Transform2D.h"
#include "../components/Sprite.h"

namespace Wiwa {
	class WI_API SpriteRenderer : public System<Transform2D, Sprite> {
	private:

	public:
		SpriteRenderer();
		~SpriteRenderer();

		void OnUpdate() override;

		void OnUpdateComponents(Transform2D* t2d, Sprite* spr){}

	protected:
		void OnEntityAdded(EntityId entityID) override;
	};
}