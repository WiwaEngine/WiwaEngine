#include "wipch.h"
#include "embed_time_functions.h"
#include <Wiwa/utilities/time/Time.h>

float GetDeltaTimeIntr()
{
    return Wiwa::Time::GetDeltaTime();
}

float GetTimeIntr()
{
    return Wiwa::Time::GetTime();
}
