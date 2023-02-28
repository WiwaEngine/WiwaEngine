#include <wipch.h>
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "ScriptClass.h"
#include "SystemClass.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/class.h>

#include "embed_functions/embed_camera_functions.h"
#include "embed_functions/embed_ecs_functions.h"
#include "embed_functions/embed_input_functions.h"
#include "embed_functions/embed_log_functions.h"
#include "embed_functions/embed_time_functions.h"
#include "embed_functions/embed_audio_functions.h"

#define WI_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Wiwa.InternalCalls::" #Name, Name)

namespace Wiwa
{
	void ScriptGlue::RegisterFunctions()
	{
		// Logging
		WI_ADD_INTERNAL_CALL(NativeLog);
		WI_ADD_INTERNAL_CALL(NativeLogVector);

		// Input
		WI_ADD_INTERNAL_CALL(IsKeyDownIntr);
		WI_ADD_INTERNAL_CALL(IsMouseButtonPressedIntr);
		WI_ADD_INTERNAL_CALL(GetMouseXIntr);
		WI_ADD_INTERNAL_CALL(GetMouseYIntr);
		WI_ADD_INTERNAL_CALL(GetMouseXDeltaIntr);
		WI_ADD_INTERNAL_CALL(GetMouseYDeltaIntr);

		// ECS
		WI_ADD_INTERNAL_CALL(GetComponent);
		WI_ADD_INTERNAL_CALL(AddComponent);
		WI_ADD_INTERNAL_CALL(ApplySystem);
		WI_ADD_INTERNAL_CALL(CreateEntity);
		WI_ADD_INTERNAL_CALL(DestroyEntity);

		//Time
		WI_ADD_INTERNAL_CALL(GetDeltaTimeIntr);
		WI_ADD_INTERNAL_CALL(GetTimeIntr);

		// Camera
		WI_ADD_INTERNAL_CALL(GetActiveCamera);
		WI_ADD_INTERNAL_CALL(SetCameraPosition);
		WI_ADD_INTERNAL_CALL(CameraLookAt);
		WI_ADD_INTERNAL_CALL(SetCameraFront);
		WI_ADD_INTERNAL_CALL(SetCameraRotation);

		// Resources
		WI_ADD_INTERNAL_CALL(LoadResourceModel);
		WI_ADD_INTERNAL_CALL(AddMeshToEntity);

		// Audio
		/*WI_ADD_INTERNAL_CALL(PlayMusic);
		WI_ADD_INTERNAL_CALL(StopMusic);

		WI_ADD_INTERNAL_CALL(PlaySound);
		WI_ADD_INTERNAL_CALL(StopSound);

		WI_ADD_INTERNAL_CALL(StopAllEvents);*/
	}
}