#include <wipch.h>
#include "embed_audio_functions.h"

#include <mono/metadata/object.h>

#include <Wiwa/audio/Audio.h>

void PlayMusic(MonoString* ev_name)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::PostEvent(ev_p);
}

void StopMusic(MonoString* ev_name)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::StopEvent(ev_p);
}

void PlaySound(MonoString* ev_name, size_t entity)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::PostEvent(ev_p, entity);
}

void StopSound(MonoString* ev_name, size_t entity)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::StopEvent(ev_p, entity);
}

void StopAllEvents()
{
	Audio::StopAllEvents();
}