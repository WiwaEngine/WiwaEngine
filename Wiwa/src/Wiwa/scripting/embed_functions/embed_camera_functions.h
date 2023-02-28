#pragma once
#include <glm/glm.hpp>

size_t GetActiveCamera();

void SetCameraPosition(size_t camid, glm::vec3 vector);

void CameraLookAt(size_t camid, glm::vec3 vec);

void SetCameraFront(size_t camid, glm::vec3 vector);

void SetCameraRotation(size_t camid, glm::vec3 angles);