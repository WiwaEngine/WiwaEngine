#pragma once
#include "../MonoDefinitions.h"
#include <glm/glm.hpp>

void NativeLog(MonoString* string, int parameter);

void NativeLogVector(glm::vec3* parameter, glm::vec3* outParam);