#pragma once

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa {
	struct Object;
	class WI_API System {
	private:

	protected:
		EntityId m_EntityId;
		Scene* m_Scene;

		template<class T> T* GetComponent();

		virtual void OnAwake(){}

		virtual void OnInit(){}

		virtual void OnUpdate() {}

		virtual void OnDestroy(){}

		virtual void OnEntitySet() {}

		virtual void OnSceneSet(){}
	public:
		System();
		virtual ~System(); // Virtual destructor, so that child destructor is called

		void SetEntity(EntityId entity) { m_EntityId = entity; OnEntitySet(); }
		void SetScene(Scene* scene) { m_Scene = scene; OnSceneSet(); }

		EntityId GetEntity() { return m_EntityId; }

		void Awake();
		void Init();
		void Update();

		virtual void OnSystemAdded() {}

		virtual void OnSystemRemoved() {}
		
		void Destroy();
		virtual void OnCollisionEnter(Object* body1, Object* body2)
		{}
		virtual void OnCollision(Object* body1, Object* body2)
		{}
		virtual void OnCollisionLeave(Object* body1, Object* body2)
		{}

		virtual bool OnComponentAdded(byte* data, const Type* type) { return true; }

		virtual bool OnComponentRemoved(byte* data, const Type* type) { return true; }
	};

	template<class T>
	inline T* System::GetComponent()
	{
		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		T* component = em.GetComponent<T>(m_EntityId);

		return component;
	}
}