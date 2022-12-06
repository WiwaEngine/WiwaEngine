#pragma once

#include <Wiwa/core/Core.h>

#include "Scene.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>

#include <vector>

typedef size_t SceneId;

namespace Wiwa {
	class WI_API SceneManager {
	private:
		static std::vector<Scene*> m_Scenes;
		static SceneId m_ActiveScene;

		static void LoadEntity(File& scene_file, EntityId parent, EntityManager& em, bool is_parent);
		static void SaveEntity(File& scene_file, EntityId eid, EntityManager& em);

		SceneManager();
	public:
		static void Update();
		static void CleanUp();
		
		static void SetScene(SceneId sceneId) { m_ActiveScene = sceneId; m_Scenes[sceneId]->Start(); }
		static void ChangeScene(SceneId sceneId);

		static void StartChangeScene(SceneId sceneId);

		static SceneId getActiveSceneId() { return m_ActiveScene; }
		static Scene* getActiveScene() { return m_Scenes[m_ActiveScene]; }

		static size_t getSceneSize() { return m_Scenes.size(); }

		static size_t CreateScene();
		static Scene* getScene(SceneId sceneId) { return m_Scenes[sceneId]; }
		static std::vector<Scene*>& getScenes() { return m_Scenes; }

		static void SaveScene(SceneId scene_id, const char* scene_path);
		static SceneId LoadScene(const char* scene_path);
	};
}