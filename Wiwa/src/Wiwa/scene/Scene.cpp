#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/utilities/render/LightManager.h>

namespace Wiwa
{
	Scene::Scene()
	{
		mMaxTimeEntering = 0;
		mMaxTimeLeaving = 0;

		m_EntityManager.SetScene(this);
		m_CameraManager = new CameraManager();
		m_LightManager = new LightManager();
		m_PhysicsManager = new PhysicsManager();

		m_PhysicsManager->InitWorld();
	}

	Scene::~Scene()
	{
		delete m_CameraManager;
		delete m_LightManager;

		// Clear entity manager
		m_EntityManager.Clear();

		// Clear physics world
		m_PhysicsManager->CleanWorld();
		delete m_PhysicsManager;
		m_PhysicsManager = nullptr;
	}

	void Scene::Start()
	{
	}

	void Scene::Awake()
	{
		m_EntityManager.SystemsAwake();
	}

	void Scene::Init()
	{
		m_EntityManager.SystemsInit();
	}

	void Scene::Update()
	{
		switch (m_CurrentState)
		{
		case Scene::SCENE_ENTERING:
			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			if (m_TransitionTimer >= mMaxTimeEntering)
				m_CurrentState = SCENE_LOOP;
			UpdateEnter();
			RenderEnter();
			break;
		case Scene::SCENE_LOOP:
			m_EntityManager.SystemsUpdate();

			ProcessInput();
			UpdateLoop();
			RenderLoop();
			break;
		case Scene::SCENE_LEAVING:
			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			if (m_TransitionTimer >= mMaxTimeLeaving)
				SceneManager::ChangeScene(m_SceneToChange);
			UpdateLeave();
			RenderLeave();
			break;
		default:
			break;
		}
	}

	void Scene::ModuleUpdate()
	{
		m_CameraManager->Update();

		m_EntityManager.Update();

		m_PhysicsManager->UpdateEngineToPhysics();

		if (SceneManager::IsPlaying())
		{
			m_PhysicsManager->StepSimulation();
			m_PhysicsManager->UpdatePhysicsToEngine();
		}
		// m_PhysicsManager->LogBodies();

		if (!SceneManager::IsPlaying())
		{
			m_EntityManager.UpdateWhitelist();
		}
	}

	void Scene::Load()
	{
		m_TransitionTimer = 0;

		m_CurrentState = SCENE_ENTERING;
	}

	void Scene::Unload(bool unload_resources)
	{
		return;
		if (unload_resources)
		{
			Wiwa::Resources::UnloadAllResources();
		}
	}

	void Scene::ChangeScene(size_t scene)
	{
		m_SceneToChange = scene;
		m_CurrentState = SCENE_LEAVING;
		m_TransitionTimer = 0;
	}
}