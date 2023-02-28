#pragma once
#include "MonoDefinitions.h"

#include <Wiwa/utilities/Reflection.h>
#include <string>

void ClearName(std::string& str);

Class* ConvertClass(MonoType* monotype, MonoClass* monoclass);

Type* ConvertType(MonoType* monotype);

void DestroyType(const Type* type);

void DestroyClass(const Class* cl);