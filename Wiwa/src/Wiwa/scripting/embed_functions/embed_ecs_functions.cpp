#include "wipch.h"
#include "embed_ecs_functions.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/metadata.h>

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/Log.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Resources.h>

#include <Wiwa/ecs/components/Mesh.h>

#include "../MonoWiwaTranslations.h"

byte* GetComponent(size_t id, MonoReflectionType* type)
{
	static std::unordered_map<size_t, Type*> s_ConvertedTypes;

	MonoType* compType = mono_reflection_type_get_type(type);
	std::string typeName = mono_type_get_name(compType);
	ClearName(typeName);
	size_t typeHash = FNV1A_HASH(typeName.c_str());

	std::unordered_map<size_t, Type*>::iterator converted_type = s_ConvertedTypes.find(typeHash);

	Type* t = NULL;

	if (converted_type == s_ConvertedTypes.end())
	{
		t = ConvertType(compType);

		s_ConvertedTypes[typeHash] = t;
	}
	else
	{
		t = converted_type->second;
	}

	int alingment;

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ComponentId compID = em.GetComponentId(t);
	byte* comp = em.GetComponent(id, compID, t->size);

	return comp;
}

size_t CreateEntity()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	return em.CreateEntity();
}

void DestroyEntity(size_t eid)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	em.DestroyEntity(eid);
}

size_t LoadResourceModel(MonoString* str)
{
	char* model = mono_string_to_utf8(str);

	return Wiwa::Resources::Load<Wiwa::Model>(model);
}

void AddMeshToEntity(size_t eid, MonoString* model, MonoString* mat)
{
	char* model_p = mono_string_to_utf8(model);
	char* mat_p = mono_string_to_utf8(mat);

	Wiwa::Mesh mesh;
	mesh.drawChildren = false;
	mesh.modelIndex = 0;
	sprintf_s(mesh.mesh_path, "%s", model_p);
	sprintf_s(mesh.mat_path, "%s", mat_p);

	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	em.AddComponent<Wiwa::Mesh>(eid, mesh);
}

byte* AddComponent(size_t id, MonoReflectionType* type)
{
	static std::unordered_map<size_t, Type*> s_ConvertedTypes;

	MonoType* compType = mono_reflection_type_get_type(type);
	std::string typeName = mono_type_get_name(compType);
	ClearName(typeName);
	size_t typeHash = FNV1A_HASH(typeName.c_str());

	std::unordered_map<size_t, Type*>::iterator converted_type = s_ConvertedTypes.find(typeHash);

	Type* t = NULL;

	if (converted_type == s_ConvertedTypes.end())
	{
		t = ConvertType(compType);

		s_ConvertedTypes[typeHash] = t;
	}
	else
	{
		t = converted_type->second;
	}

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ComponentId compID = em.GetComponentId(t);

	byte* comp = em.AddComponent(id, t, NULL);

	return comp;
}

void ApplySystem(size_t id, MonoReflectionType* type)
{
	MonoType* compType = mono_reflection_type_get_type(type);
	std::string typeName = mono_type_get_name(compType);
	ClearName(typeName);
	size_t typeHash = FNV1A_HASH(typeName.c_str());

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	em.ApplySystem(id, typeHash);
}
