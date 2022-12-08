#pragma once

#include <Wiwa/core/Application.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>

inline void CreateNew3DEnt()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId myEntity = em.CreateEntity("New Entity");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
}

inline void CreateNewChild(EntityId parentId)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId myEntity = em.CreateEntity("New entity", parentId);

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
}

inline void CreateCube()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Cube");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "cube");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreatePlane()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Plane");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "plane");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreatePyramid()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Pyramid");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "pyramid");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreateSphere()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	EntityId myEntity = em.CreateEntity("Sphere");

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };

	em.AddComponent<Wiwa::Transform3D>(myEntity, t3d);
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", "resources/meshes/sphere.fbx");
	sprintf(mesh.mat_path, "%s", "resources/materials/default_material.wimaterial");
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);
	em.AddComponent<Wiwa::Mesh>(myEntity, mesh);
	em.ApplySystem<Wiwa::MeshRenderer>(myEntity);
}

inline void CreateEntityWithModelHiearchy(const char* model_path, const char* material_path) {
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	// Prepare mesh data
	Wiwa::Mesh mesh;
	sprintf(mesh.mesh_path, "%s", model_path);
	sprintf(mesh.mat_path, "%s", material_path);
	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);

	// Prepare default transform
	Wiwa::Transform3D t3d;
	t3d.position = { 0.0f, 0.0f, 0.0f };
	t3d.localPosition = { 0.0f, 0.0f, 0.0f };
	t3d.rotation = { 0.0f,0.0f, 0.0f };
	t3d.scale = { 1.0f, 1.0f, 1.0f };

	// Take root model
	Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh.meshId);

	// Create entity with model's hierarchy
	const Wiwa::ModelHierarchy* model_h = model->getModelHierarchy();
	size_t children_size = model_h->children.size();

	EntityId e_root = em.CreateEntity(model_h->name.c_str());
	em.AddComponent<Wiwa::Transform3D>(e_root, t3d);

	auto process_h = [&](const Wiwa::ModelHierarchy* m_h, EntityId parent, auto&& process_h) -> void {
		EntityId e_child = em.CreateEntity(m_h->name.c_str(), parent);
		em.AddComponent(e_child, t3d);

		size_t c_mesh_size = m_h->meshIndexes.size();
		size_t c_child_size = m_h->children.size();

		if (c_mesh_size > 0) {
			mesh.modelIndex = m_h->meshIndexes[0];
			em.AddComponent(e_child, mesh);
			em.ApplySystem<Wiwa::MeshRenderer>(e_child);
		}

		if (c_child_size > 0) {
			for (size_t i = 0; i < c_child_size; i++) {
				process_h(m_h->children[i], e_child, process_h);
			}
		}
	};

	for (size_t i = 0; i < children_size; i++) {
		const Wiwa::ModelHierarchy* child_h = model_h->children[i];

		process_h(child_h, e_root, process_h);
	}
}