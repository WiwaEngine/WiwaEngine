#pragma once

extern "C" {
	typedef struct _MonoType MonoType;
	typedef struct _MonoClass MonoClass;

	// object.h
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoReflectionMethod MonoReflectionMethod;
	typedef struct _MonoReflectionModule MonoReflectionModule;
	typedef struct _MonoReflectionField MonoReflectionField;
	typedef struct _MonoReflectionProperty MonoReflectionProperty;
	typedef struct _MonoReflectionEvent MonoReflectionEvent;
	typedef struct _MonoReflectionType MonoReflectionType;
	typedef struct _MonoDelegate MonoDelegate;
	typedef struct _MonoThreadsSync MonoThreadsSync;
	typedef struct _MonoThread MonoThread;
	typedef struct _MonoDynamicAssembly MonoDynamicAssembly;
	typedef struct _MonoDynamicImage MonoDynamicImage;
	typedef struct _MonoReflectionMethodBody MonoReflectionMethodBody;
	typedef struct _MonoAppContext MonoAppContext;
}