#pragma once

#include "../MonoDefinitions.h"

void PlayMusic(MonoString* ev_name);
void StopMusic(MonoString* ev_name);

void PlaySound(MonoString* ev_name, size_t entity);
void StopSound(MonoString* ev_name, size_t entity);

void StopAllEvents();