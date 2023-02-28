#include <wipch.h>
#include "MonoWiwaTranslations.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/class.h>

void ClearName(std::string& str) {
	size_t ind = str.find('.');

	if (ind != str.npos)
	{
		str = str.substr(ind + 1, str.size() - ind - 1);
	}
}

Class* ConvertClass(MonoType* monotype, MonoClass* monoclass)
{
	Class* t = new Class();
	t->is_class = true;
	t->is_array = false;
	t->is_enum = false;

	int alignment;

	// Fields
	t->field_count = mono_class_num_fields(monoclass);

	MonoClassField* monofield = NULL;
	MonoClass* monofieldclass = NULL;

	void* iter = NULL;
	size_t offset = 0;

	for (size_t i = 0; i < t->field_count; i++)
	{
		monofield = mono_class_get_fields(monoclass, &iter);
		monotype = mono_field_get_type(monofield);

		Field field;
		field.name = mono_field_get_name(monofield);
		field.offset = offset;
		field.type = ConvertType(monotype);

		t->fields.push_back(field);

		offset += mono_type_size(monotype, &alignment);
	}

	return t;
}

Type* ConvertType(MonoType* monotype)
{
	MonoClass* monoclass = mono_type_get_class(monotype);

	Type* type = NULL;

	if (monoclass)
	{
		type = ConvertClass(monotype, monoclass);
	}
	else
	{
		type = new Type();
		type->is_enum = false;
		type->is_array = false;
		type->is_class = false;
	}

	int alignment = 0;

	std::string name = mono_type_get_name(monotype);
	ClearName(name);

	// Translate C# to C++
	if (name == "Single")
	{
		name = "float";
	}
	else if (name == "UInt32")
	{
		name = "unsigned int";
	}
	else if (name == "UInt64")
	{
		name = "unsigned __int64";
	}
	else if (name == "Int32")
	{
		name = "int";
	}
	else if (name == "Int64")
	{
		name = "__int64";
	}

	type->name = name;
	type->size = mono_type_size(monotype, &alignment);
	type->hash = std::hash<std::string>{}(type->name);

	return type;
}

void DestroyType(const Type* type)
{
	if (type->is_class)
	{
		DestroyClass((Class*)type);
	}

	delete type;
}

void DestroyClass(const Class* cl) {
	for (size_t i = 0; i < cl->field_count; i++)
	{
		DestroyType(cl->fields[i].type);
	}
}