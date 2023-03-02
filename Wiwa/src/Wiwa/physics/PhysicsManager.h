#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/components/RigidBody.h>
#include <Wiwa/ecs/components/ColliderCube.h>
#include <Wiwa/ecs/components/ColliderCylinder.h>
#include <Wiwa/ecs/components/ColliderSphere.h>
#include <Wiwa/ecs/components/ColliderCapsule.h>
#include "Wiwa/ecs/components/Transform3D.h"
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/render/Uniforms.h>
#include <Wiwa/core/Resources.h>
#include <reactphysics3d/reactphysics3d.h>

#include <glm/glm.hpp>

#include <vector>
#include <bitset>
#include <algorithm>

#define MAX_BITS 32
// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY glm::vec3(0.0f, -10.0f, 0.0f)
class Camera;

using namespace reactphysics3d;

// Your event listener class
class CustomEventListener : public EventListener {

	/// Called when some contacts occur
		/**
		 * @param callbackData Contains information about all the contacts
		 */
	void onContact(const CollisionCallback::CallbackData& callbackData);

	/// Called when some trigger events occur
	/**
	 * @param callbackData Contains information about all the triggers that are colliding
	 */
	void onTrigger(const OverlapCallback::CallbackData& callbackData);
};

namespace Wiwa {

	struct Object {
		Object(RigidBody& body_, const size_t id_) : m_RigidBody(&body_), id(id_) {};

		RigidBody* m_RigidBody;
		size_t id;
	};

	enum CollisionType {
		CT_ENTER,
		CT_LOOP,
		CT_LEAVE,
		CT_NONE
	};

	struct CollisionData {
		size_t entityA;
		size_t entityB;
		CollisionType collisionType;
	};

	class WI_API PhysicsManager
	{
	public:
		PhysicsManager();

		~PhysicsManager();

		// Init & Delete World
		bool InitWorld();

		bool StepSimulation();

		bool UpdateEngineToPhysics();

		bool UpdatePhysicsToEngine();

		bool CleanWorld();

		bool DeleteBody(Object* body);

		// Add bodies
		bool AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCapsule(size_t id, const Wiwa::ColliderCapsule& capsule, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		Object* FindByEntityId(size_t id);

		bool OnSave();

		bool OnLoad();

		bool getInit();

		void DebugDrawWorld();

		void DrawLine(const reactphysics3d::Vector3& from, const reactphysics3d::Vector3& to/*, const reactphysics3d::Vector3& color*/);

		bool AddFilterTag(const char* str);
		
		void RemoveFilterTag(const int index);

	private:
		bool m_Debug;
		bool m_HasBeenInit;
		long double accumulator = 0.0;

		reactphysics3d::PhysicsCommon m_PhysicsCommon;
		PhysicsWorld* m_World;
		CustomEventListener m_CollisionListener;

		std::list<Object*> m_Bodies;
	private:
		std::list<Object*> m_BodiesToLog;

	public:
		bool AddBodyToLog(Object* body_to_log);
		bool RemoveBodyFromLog(Object* body_to_log);
		bool LogBodies();

	public:
		std::vector<std::string> filterStrings;
		std::vector<std::bitset<MAX_BITS>> fliterBitsSets;

		void bin(unsigned n)
		{
			if (n > 1)
				bin(n >> 1);

			WI_INFO("{}", n & 1);
		}

		ResourceId lineDisplayShaderId;
		Wiwa::Shader* lineDisplayShader;
		DefaultUnlitUniforms lineDisplayShaderUniforms;
	};

	/*class MyMemory : public reactphysics3d::MemoryAllocator
	{
		/// Allocate memory of a given size (in bytes) and return a pointer to the
		/// allocated memory.
		void* allocate(size_t size) override;

		/// Release previously allocated memory.
		void release(void* pointer, size_t size) override;
	};*/

	void ToEulerAngles(const reactphysics3d::Quaternion& q, glm::vec3& angles);
}

