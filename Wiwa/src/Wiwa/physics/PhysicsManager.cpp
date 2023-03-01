#include <wipch.h>
#include "PhysicsManager.h"
#include "Wiwa/ecs/systems/System.h"
#include "Wiwa/ecs/EntityManager.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/utilities/render/CameraManager.h"
#include <glew.h>

#include <Wiwa/utilities/Log.h>
#include "Wiwa/utilities/time/Time.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Wiwa/utilities/json/JSONDocument.h>

namespace Wiwa {
	PhysicsManager::PhysicsManager()
	{
		m_HasBeenInit = false;

		// Bullet Bounding box
		lineDisplayShaderId = Resources::Load<Shader>("resources/shaders/debug/line_display");
		lineDisplayShader = Resources::GetResourceById<Shader>(lineDisplayShaderId);
		lineDisplayShader->Compile("resources/shaders/debug/line_display");
		lineDisplayShaderUniforms.Model = lineDisplayShader->getUniformLocation("u_Model");
		lineDisplayShaderUniforms.View = lineDisplayShader->getUniformLocation("u_View");
		lineDisplayShaderUniforms.Projection = lineDisplayShader->getUniformLocation("u_Proj");
	}

	PhysicsManager::~PhysicsManager()
	{

	}

	bool PhysicsManager::InitWorld()
	{
		m_Debug = true;
		m_HasBeenInit = true;

		// Create the world settings
		PhysicsWorld::WorldSettings settings;
		settings.defaultVelocitySolverNbIterations = 20;
		settings.isSleepingEnabled = false;
		settings.gravity = Vector3(0, -10, 0);

		PhysicsWorld* world = Application::Get().GetPhysics().createPhysicsWorld();
		// Change the number of iterations of the velocity solver
		world->setNbIterationsVelocitySolver(15);
		// Change the number of iterations of the position solver
		world->setNbIterationsPositionSolver(8);
		// Register your event listener class
		world->setEventListener(&m_CollisionListener);
		// Enable debug rendering
		world->setIsDebugRenderingEnabled(true);

		// Get a reference to the debug renderer
		DebugRenderer& debugRenderer = world->getDebugRenderer();
		// Select the contact points and contact normals to be
		//displayed
			debugRenderer.setIsDebugItemDisplayed(DebugRenderer ::
				DebugItem::CONTACT_POINT, true);
		debugRenderer.setIsDebugItemDisplayed(DebugRenderer ::
			DebugItem::CONTACT_NORMAL, true);
		debugRenderer.setIsDebugItemDisplayed(DebugRenderer ::
			DebugItem::COLLISION_SHAPE, true);
		debugRenderer.setIsDebugItemDisplayed(DebugRenderer ::
			DebugItem::COLLIDER_AABB, true);

		AddFilterTag("COLLISION_EVERYTHING");
		WI_INFO("Physics Manager Init");
		
		return true;
	}

	bool PhysicsManager::StepSimulation()
	{
		// Constant physics time step
		const float timeStep = 1.0f / 60.0f;
		// Compute the time difference between the two frames
		long double deltaTime = Time::GetDeltaTimeSeconds();
		// Add the time difference in the accumulator
		accumulator += deltaTime;

		// While there is enough accumulated time to take
		// one or several physics steps
		while (accumulator >= timeStep) {
			// Update the Dynamics world with a constant time step
			m_World->update(timeStep);
			// Decrease the accumulated time
			accumulator -= timeStep;
		}		
		return true;
	}

	bool PhysicsManager::UpdateEngineToPhysics()
	{
		// Set the position offset
		// Get the position from the engine
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		for (std::list<Object*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			Transform3D*transform3d = entityManager.GetComponent<Wiwa::Transform3D>((*item)->id);
			Rigidbody*rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>((*item)->id);

			// Get the position from the engine
			glm::vec3 posEngine = transform3d->localPosition;
			glm::quat rotEngine = glm::quat(transform3d->localMatrix);
			//glm::quat rotEngine = entityData->transform3d->rotation;

			// Get the offset
			glm::vec3 finalOffset = rotEngine * rigidBody->positionOffset;
			glm::vec3 finalPosReact = posEngine + finalOffset;

			// Apply the offset because offset, it is internal only(collider wise)
			Transform offsettedCollider;
			offsettedCollider.setFromOpenGL(glm::value_ptr(transform3d->localMatrix));
			offsettedCollider.setPosition(Vector3(finalPosReact.x, finalPosReact.y, finalPosReact.z));
			offsettedCollider.setOrientation(Quaternion(rotEngine.x, rotEngine.y, rotEngine.z, rotEngine.w));

			//(*item)->m_CollisionObject->getCollisionShape()->setLocalScaling((btVector3(rigidBody->scalingOffset.x, rigidBody->scalingOffset.y, rigidBody->scalingOffset.z)));
			(*item)->m_RigidBody->setTransform(offsettedCollider);
		}
		return true;
	}

	bool PhysicsManager::UpdatePhysicsToEngine()
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		// Physics to Engine
		for (std::list<Object*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			Transform3D* transform3d = entityManager.GetComponent<Wiwa::Transform3D>((*item)->id);
			Rigidbody* rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>((*item)->id);

			Transform reactTransform((*item)->m_RigidBody->getTransform());

			// Get the transform from physics world
			Vector3 posReact = reactTransform.getPosition();
			Quaternion rotationReact = reactTransform.getOrientation();

			glm::quat rotationEngine = glm::quat(rotationReact.w, rotationReact.x, rotationReact.y, rotationReact.z);
			glm::vec3 finalOffset = rotationEngine * rigidBody->positionOffset;
			glm::vec3 posEngine = glm::vec3(posReact.x - finalOffset.x, posReact.y - finalOffset.y, posReact.z - finalOffset.z);

			// Remove the offset because offset is internal only(collider wise)
			transform3d->localPosition = posEngine;
			reactTransform.setPosition(Vector3(posEngine.x, posEngine.y, posEngine.z));
			//bulletTransform.setRotation(rotationBullet);
			glm::vec3 eulerAngles;
			ToEulerAngles(reactTransform.getOrientation(), eulerAngles);
			transform3d->localRotation = glm::degrees(eulerAngles);
			/*bulletTransform.getOpenGLMatrix(glm::value_ptr(entityData->transform3d->localMatrix));*/

			/*(*item)->m_CollisionObject->getCollisionShape()->setLocalScaling((btVector3(rigidBody->scalingOffset.x, rigidBody->scalingOffset.y, rigidBody->scalingOffset.z)));
			m_World->updateSingleAabb((*item)->m_CollisionObject);*/
		}

		return true;
	}

	bool PhysicsManager::CleanWorld()
	{
		for (std::list<Object*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			m_World->destroyRigidBody((*item)->m_RigidBody);
		}
		m_Bodies.clear();

		Application::Get().GetPhysics().destroyPhysicsWorld(m_World);

		m_HasBeenInit = false;
		return true;
	}

	bool PhysicsManager::DeleteBody(Object* body)
	{
		m_World->destroyRigidBody(body->m_RigidBody);
		return true;
	}

	bool PhysicsManager::AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::vec3 scaling;
		glm::quat rotation;
		glm::vec3 translation;
		glm::decompose(transform.worldMatrix, scaling, rotation, translation, skew, perspective);

		// Initial position and orientation of the rigid body
		Vector3 position(transform.localPosition.x, transform.localPosition.y, transform.localPosition.z);
		Quaternion orientation = Quaternion(rotation.x, rotation.y, rotation.z, rotation.w);
		Transform transformReact(position, orientation);

		// Create a rigid body in the world
		RigidBody* body = m_World->createRigidBody(transformReact);
		body->setMass(rigid_body.mass);
		if (rigid_body.isStatic)
			body->setType(BodyType::STATIC);// Change the type of the body to kinematic

		// Create the sphere shape with a radius of 2m
		SphereShape* sphereShape = Application::Get().GetPhysics().createSphereShape
		(sphere.radius);

		// Relative transform of the collider relative to the body
		//origin
		Transform transformCollider = Transform::identity();
		// Add the collider to the rigid body
		Collider* collider;
		collider = body->addCollider(sphereShape, transformCollider);
		collider->setIsTrigger(rigid_body.isTrigger);
		return true;
	}

	bool PhysicsManager::AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::vec3 scaling;
		glm::quat rotation;
		glm::vec3 translation;
		glm::decompose(transform.worldMatrix, scaling, rotation, translation, skew, perspective);

		// Initial position and orientation of the rigid body
		Vector3 position(transform.localPosition.x, transform.localPosition.y, transform.localPosition.z);
		Quaternion orientation = Quaternion(rotation.x, rotation.y, rotation.z, rotation.w);
		Transform transformReact(position, orientation);

		// Create a rigid body in the world
		RigidBody* body = m_World->createRigidBody(transformReact);
		body->setMass(rigid_body.mass);
		if (rigid_body.isStatic)
			body->setType(BodyType::STATIC);// Change the type of the body to kinematic

		// Half extents of the box in the x, y and z directions
		const Vector3 halfExtents(cube.halfExtents.x, cube.halfExtents.y, cube.halfExtents.z);
		// Create the box shape
		BoxShape* boxShape = Application::Get().GetPhysics().createBoxShape(
			halfExtents);

		// Relative transform of the collider relative to the body
		//origin
			Transform transformCollider = Transform::identity();
		// Add the collider to the rigid body
		Collider* collider;
		collider = body->addCollider(boxShape, transformCollider);
		collider->setIsTrigger(rigid_body.isTrigger);
		return true;
	}

	bool PhysicsManager::AddBodyCapsule(size_t id, const Wiwa::ColliderCapsule& capsule, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::vec3 scaling;
		glm::quat rotation;
		glm::vec3 translation;
		glm::decompose(transform.worldMatrix, scaling, rotation, translation, skew, perspective);

		// Initial position and orientation of the rigid body
		Vector3 position(transform.localPosition.x, transform.localPosition.y, transform.localPosition.z);
		Quaternion orientation = Quaternion(rotation.x, rotation.y, rotation.z, rotation.w);
		Transform transformReact(position, orientation);

		// Create a rigid body in the world
		RigidBody* body = m_World->createRigidBody(transformReact);
		body->setMass(rigid_body.mass);
		if (rigid_body.isStatic)
			body->setType(BodyType::STATIC);// Change the type of the body to kinematic

		// Create the capsule shape
		CapsuleShape* capsuleShape = Application::Get().GetPhysics().createCapsuleShape(capsule.radius, capsule.height);

		// Relative transform of the collider relative to the body
		//origin
		Transform transformCollider = Transform::identity();
		// Add the collider to the rigid body
		Collider* collider;
		collider = body->addCollider(capsuleShape, transformCollider);
		collider->setIsTrigger(rigid_body.isTrigger);
		return true;
	}

	Object* PhysicsManager::FindByEntityId(size_t id)
	{
		for (std::list<Object*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			if ((*item)->id == id)
				return *item;
		}
		return nullptr;
	}

	bool PhysicsManager::OnSave()
	{
		JSONDocument physics;
		int count = filterStrings.size();

		physics.AddMember("tags_count", count);
		std::string tag = "tag_";
		for (int i = 0; i < count; i++)
		{
			std::string newTag = tag;
			newTag += std::to_string(i);
			physics.AddMember(newTag.c_str(), filterStrings[i].c_str());
		}
		std::string path = "assets/Scenes/";
		path += SceneManager::getActiveScene()->getName();
		path += "_physics.json";
		physics.save_file(path.c_str());
		return true;
	}

	bool PhysicsManager::OnLoad()
	{
		std::string path = "assets/Scenes/";
		path += SceneManager::getActiveScene()->getName();
		path += "_physics.json";

		JSONDocument physics;
		if (!physics.load_file(path.c_str()))
			return false;

		for (int i = 1; i < filterStrings.size(); i++)
			RemoveFilterTag(i);

		int count = physics["tags_count"].get<int>();
		std::string tag = "tag_";
		for (int i = 1; i < count; i++)
		{
			std::string newTag = tag;
			newTag += std::to_string(i);
			AddFilterTag(physics[newTag.c_str()].get<const char*>());
		}
		return false;
	}

	bool PhysicsManager::getInit()
	{
		return m_HasBeenInit;
	}

	void PhysicsManager::DebugDrawWorld()
	{
		Camera* camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind(false);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(camera->getProjection()));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(camera->getView()));

		//m_Debug_draw->lineDisplayShader->Bind();
		////m_Debug_draw->lineDisplayShader->setUniform(m_Debug_draw->lineDisplayShaderUniforms.Model, transform);
		//m_Debug_draw->lineDisplayShader->setUniform(m_Debug_draw->lineDisplayShaderUniforms.View, camera->getView());
		//m_Debug_draw->lineDisplayShader->setUniform(m_Debug_draw->lineDisplayShaderUniforms.Projection, camera->getProjection());
		DebugRenderer& debugRenderer = m_World->getDebugRenderer();
		reactphysics3d::Array<DebugRenderer::DebugLine> list = debugRenderer.getLines();
		
		for (int i = 0; i < list.size(); i++)
		{
			DrawLine(list[i].point1, list[i].point2);
			
		}

		//m_Debug_draw->lineDisplayShader->UnBind();
		camera->frameBuffer->Unbind();
	}

	void PhysicsManager::DrawLine(const reactphysics3d::Vector3& from, const reactphysics3d::Vector3& to/* const reactphysics3d::Vector3& color*/)
	{
		glUseProgram(0);
		glColor3f(255, 0, 0);
		glLineWidth(3.0f);
		glBegin(GL_LINES);
		glVertex3f(from.x, from.y, from.z);
		glVertex3f(to.x, to.y, to.z);
		glEnd();
		glLineWidth(1.0f);
	}

	bool PhysicsManager::AddFilterTag(const char* str)
	{
		if (filterStrings.size() == 32)
			return false;

		filterStrings.emplace_back(str);
		std::bitset<MAX_BITS> bset;
		bset.set(filterStrings.size(), true);
		fliterBitsSets.push_back(bset);
		return true;
	}
	void PhysicsManager::RemoveFilterTag(const int index)
	{
		filterStrings.erase(filterStrings.begin() + index);
		fliterBitsSets.erase(fliterBitsSets.begin() + index);
	}

	bool PhysicsManager::AddBodyToLog(Object* body_to_log)
	{
		m_BodiesToLog.emplace_back(body_to_log);
		return true;
	}

	bool PhysicsManager::RemoveBodyFromLog(Object* body_to_log)
	{
		m_BodiesToLog.remove(body_to_log);
		return true;
	}

	bool PhysicsManager::LogBodies()
	{
		if (m_BodiesToLog.empty())
			return false;

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* name = Wiwa::SceneManager::getActiveScene()->getName();
		//WI_INFO("SCENE {} World has total of {} bodies", name, m_Bodies.size());
		int num = 0;
		for (std::list<Object*>::iterator item = m_BodiesToLog.begin(); item != m_BodiesToLog.end(); item++)
		{
			//btVector3 pos = (*item)->m_CollisionObject->getWorldTransform().getOrigin();
			///*const char* e_name = entityManager.GetEntityName(id); */
			//WI_INFO("Id {} is at {} {} {}", (*item)->id, pos.x(), pos.y(), pos.z()); 
			//num++;
		}
		return true;
	}
}

//void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
//{
//	WI_INFO("Line from {} {} {} to {} {} {}", from.x(), from.y(), from.z(), to.x(), to.y(), to.z());
//
//	// Create Vertex Array Object
//	GLuint vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	// Create a Vertex Buffer Object and copy the vertex data to it
//	GLuint vbo;
//	glGenBuffers(1, &vbo);
//	GLfloat lineVertices[] = {
//		from.x(), from.y(), from.z(),
//		to.x(), to.y(), to.z()
//	};
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineVertices, GL_STATIC_DRAW);
//
//	// Create an element array
//	GLuint ebo;
//	glGenBuffers(1, &ebo);
//	GLuint elements[] = {
//		0, 1,
//	};
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), elements, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lineVertices), (void*)0);
//	
//	glBindVertexArray(vao);
//	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	glDeleteVertexArrays(1, &vao);
//	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &ebo);
//}

//void DebugDrawer::drawContactPoint(const btVector3& point_onB, const btVector3& normal_onB, btScalar distance, int life_time, const btVector3& color)
//{
//	//point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
//	//point.color.Set(color.getX(), color.getY(), color.getZ());
//	//point.Render();
//}

void CustomEventListener::onContact(const CollisionCallback::CallbackData& callbackData)
{
	Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	Wiwa::PhysicsManager& physics = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
	// For each contact pair
	for (uint p = 0; p < callbackData.getNbContactPairs(); p
		++) {
		// Get the contact pair
		CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

		Wiwa::Object* obj1 = (Wiwa::Object*)contactPair.getBody1()->getUserData();
		Wiwa::Object* obj2 = (Wiwa::Object*)contactPair.getBody2()->getUserData();

		std::vector<Wiwa::System*>& vecA = entityManager.GetEntitySystems(obj1->id);
		std::vector<Wiwa::System*>& vecB = entityManager.GetEntitySystems(obj2->id);

		switch (contactPair.getEventType())
		{
		case CollisionCallback::ContactPair::EventType::ContactStart:
			for (size_t k = 0; k < vecA.size(); k++) {
				vecA[k]->OnCollisionEnter(obj1, obj2);
			}
			for (size_t j = 0; j < vecA.size(); j++) {
				vecA[j]->OnCollisionEnter(obj2, obj1);
			}
			break;
		case CollisionCallback::ContactPair::EventType::ContactStay:
			for (size_t k = 0; k < vecA.size(); k++) {
				vecA[k]->OnCollision(obj1, obj2);
			}
			for (size_t j = 0; j < vecA.size(); j++) {
				vecA[j]->OnCollision(obj2, obj1);
			}
			break;
		case CollisionCallback::ContactPair::EventType::ContactExit:
			for (size_t k = 0; k < vecA.size(); k++) {
				vecA[k]->OnCollisionLeave(obj1, obj2);
			}
			for (size_t j = 0; j < vecA.size(); j++) {
				vecA[j]->OnCollisionLeave(obj2, obj1);
			}
			break;
		default:
			break;
		}
	}
}

void CustomEventListener::onTrigger(const OverlapCallback::CallbackData&)
{
}
