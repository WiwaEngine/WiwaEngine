#include <wipch.h>
#include "Wiwa/ecs/EntityManager.h"
#include "Wiwa/scene/SceneManager.h"
#include "ParticleManager.h"
#include <iostream>
#include <Wiwa/utilities/filesystem/FileSystem.h>

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

bool ParticleManager::CreateCustomParticle(Wiwa::ParticleComponent* particleComponent)
{
	return true;
}

//Function to add update the billboard should go here
//Another function to draw the Billboard should also go here

bool ParticleManager::UpdateBillBoard(Wiwa::Billboard* component)
{
	/*
	switch (billboard_alignment)
	{
	case Billboarding_Alignment::SCREEN_ALIGNED:
		ScreenAlign();
		break;
	case Billboarding_Alignment::WORLD_ALIGNED:
		WorldAlign();
		break;
	case Billboarding_Alignment::AXIS_ALIGNED:
		AxisAlign();
		break;
	default:
		break;
	}
	*/
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


