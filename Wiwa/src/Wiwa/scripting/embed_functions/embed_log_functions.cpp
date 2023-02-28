#include "wipch.h"
#include "embed_log_functions.h"

#include <mono/metadata/object.h>
#include <Wiwa/utilities/Log.h>

void NativeLog(MonoString* string, int parameter)
{
	char* str = mono_string_to_utf8(string);

	WI_CORE_TRACE("{0} {1}", str, parameter);
	mono_free(str);
}

void NativeLogVector(glm::vec3* parameter, glm::vec3* outParam)
{
	WI_CORE_TRACE("Value of X:{0}, Y:{1}, Z{2}", parameter->x, parameter->y, parameter->z);
	*outParam = glm::normalize(*parameter);
}