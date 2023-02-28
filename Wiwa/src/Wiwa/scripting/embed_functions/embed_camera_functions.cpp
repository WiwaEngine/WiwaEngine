#include "wipch.h"
#include "embed_camera_functions.h"

#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/scene/SceneManager.h>

size_t GetActiveCamera()
{
	Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();

	return cm.getActiveCameraId();
}

void SetCameraPosition(size_t camid, glm::vec3 vector)
{
	Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
	Wiwa::Camera* cam = cm.getCamera(camid);
	cam->setPosition(vector);
}

void CameraLookAt(size_t camid, glm::vec3 vec)
{
	Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
	Wiwa::Camera* cam = cm.getCamera(camid);
	cam->lookat(vec);
}

void SetCameraFront(size_t camid, glm::vec3 vector)
{
	Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
	Wiwa::Camera* cam = cm.getCamera(camid);
	cam->setFront(vector);
}

void SetCameraRotation(size_t camid, glm::vec3 angles)
{
	Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
	Wiwa::Camera* cam = cm.getCamera(camid);
	cam->setRotation({ angles.x, angles.y, angles.z });
}
