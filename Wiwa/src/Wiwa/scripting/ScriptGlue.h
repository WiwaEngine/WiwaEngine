#pragma once

extern "C" {
	typedef struct _MonoType MonoType;
}

namespace Wiwa {	
	class ScriptGlue 
	{
	public:
		static void RegisterFunctions();
	};
}