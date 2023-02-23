#include <wipch.h>
#include "Wiwa/ecs/EntityManager.h"
#include "Wiwa/scene/SceneManager.h"
#include "ParticleManager.h"
#include <iostream>
#include <Wiwa/utilities/filesystem/FileSystem.h>

namespace Wiwa {
	ParticleManager::ParticleManager()
	{

	}

	bool ParticleManager::Init()
	{
		return true;
	}

	bool ParticleManager::Update()
	{

		return true;
	}

	bool ParticleManager::Terminate()
	{
		return true;
	}

	ParticleManager::Emitter* ParticleManager::FindByEntityId(size_t id)
	{
		for (std::list<Emitter*>::iterator item = emmiters.begin(); item != emmiters.end(); item++)
		{
			if ((*item)->id == id)
				return *item;
		}
		return nullptr;
	}

	bool ParticleManager::DeleteEmitter(Emitter* emitter)
	{
		//Here we can delete the emmiter

		return true;
	}

	bool ParticleManager::CreateCustomParticle(Wiwa::ParticleComponent* particleComponent)
	{
		return true;
	}

	//Function to add update the billboard should go here
	//Another function to draw the Billboard should also go here

	bool ParticleManager::UpdateBillBoard(Wiwa::Billboard* component)
	{

		//Here to add the Screen Align for the Billboard

		return true;
	}

	bool ParticleManager::DrawBillboard(Wiwa::Billboard* component)
	{
		/*
		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		size_t cameraCount = man.getCameraSize();
		std::vector<CameraId>& cameras = man.getCameras();

		// Iterar cada camara en el engine
		for (size_t i = 0; i < cameraCount; i++)
		{
			CameraId cam_id = cameras[i];
			Camera* camera = man.getCamera(cam_id);
			glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
			camera->frameBuffer->Bind();

			// Update transformations
			// Draw after update

			camera->frameBuffer - Unbind();
		}
		*/

		return true;
	}

	bool ParticleManager::AddBillboard(Wiwa::Billboard* component)
	{
		return true;
	}

	void Wiwa::ParticleManager::Emitter::setRotation(const glm::vec3 rot)
	{
		billboardRotation = rot;

		glm::vec3 direction;
		direction.x = cos(glm::radians(rot.x)) * cos(glm::radians(rot.y));
		direction.y = sin(glm::radians(rot.y));
		direction.z = sin(glm::radians(rot.x)) * cos(glm::radians(rot.y));

		glm::vec3 front = glm::normalize(direction);
		//setFront({ front.x, front.y, front.z });
	}
}