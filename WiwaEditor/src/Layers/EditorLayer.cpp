#include "EditorLayer.h"

#include <imgui.h>
#include <stdio.h>

#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/json/JSONDocument.h>

#include <Wiwa/core/Input.h>
#include <ImGuizmo.h>

#include <imgui_internal.h>

#include <Wiwa/core/Resources.h>

#include <Wiwa/core/ProjectManager.h>
#include <Wiwa/Platform/Windows/WindowsPlatformUtils.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/components/Sprite.h>
#include <Wiwa/scripting/ScriptEngine.h>

#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/core/Renderer3D.h>
#include <Wiwa/audio/Audio.h>

#include "../Entities.h"
EditorLayer* EditorLayer::s_Instance = nullptr;
std::string EditorLayer::s_SolVersion = "vs2022";
std::string EditorLayer::s_BuildConf = "Release";
std::thread* EditorLayer::s_RegenThread;
EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	WI_CORE_ASSERT(!s_Instance, "Application already exists!");
	RegenSol();
	s_Instance = this;
	// Editor scene
	m_EditorSceneId = Wiwa::SceneManager::CreateScene();
	Wiwa::SceneManager::StopScene();

	m_EditorScene = Wiwa::SceneManager::getScene(m_EditorSceneId);
	m_EditorScene->GetEntityManager().AddSystemToWhitelist(FNV1A_HASH("MeshRenderer"));

	Wiwa::SceneManager::SetScene(m_EditorSceneId);

	Wiwa::Size2i &res = Wiwa::Application::Get().GetTargetResolution();
	float ar = res.w / (float)res.h;
	CameraId cam_id = m_EditorScene->GetCameraManager().CreatePerspectiveCamera(45, ar, 0.1f, 131.0f);
	Wiwa::Camera *cam = m_EditorScene->GetCameraManager().getCamera(cam_id);
	cam->setPosition(glm::vec3{-52.5f, 30.2f, 26.2f});
	cam->setRotation(glm::vec3{-26.0f, -30.2f, 0.0f});

	m_Configuration = std::make_unique<ConfigurationPanel>(this);
	m_Console = std::make_unique<ConsolePanel>(this);
	m_Scene = std::make_unique<ScenePanel>(this);
	m_Hierarchy = std::make_unique<HierarchyPanel>(this);
	m_Assets = std::make_unique<AssetsPanel>(this);
	m_Inspector = std::make_unique<InspectorPanel>(this);
	m_MeshView = std::make_unique<MeshViewPanel>(this);
	m_MaterialEditor = std::make_unique<MaterialPanel>(this);
	m_SystemsPanel = std::make_unique<SystemsPanel>(this);
	m_GamePanel = std::make_unique<GamePanel>(this);
	m_CamerasPanel = std::make_unique<CamerasPanel>(this);
	m_ResourcesPanel = std::make_unique<ResourcesPanel>(this);
	m_ImportPanel = std::make_unique<ImportPanel>(this);
	m_ShaderPanel = std::make_unique<ShaderPanel>(this);
	m_EnvPanel = std::make_unique<EnvironmentPanel>(this);

	m_AudioPanel = std::make_unique<AudioPanel>(this);

	m_ProjectPanel = std::make_unique<ProjectPanel>(this);
	m_About = std::make_unique<AboutPanel>(this);

	m_Panels.push_back(m_Configuration.get());
	m_Panels.push_back(m_Console.get());
	m_Panels.push_back(m_Scene.get());
	m_Panels.push_back(m_Hierarchy.get());
	m_Panels.push_back(m_Assets.get());
	m_Panels.push_back(m_Inspector.get());
	m_Panels.push_back(m_MeshView.get());
	m_Panels.push_back(m_MaterialEditor.get());
	m_Panels.push_back(m_SystemsPanel.get());
	m_Panels.push_back(m_GamePanel.get());
	m_Panels.push_back(m_CamerasPanel.get());
	m_Panels.push_back(m_ResourcesPanel.get());
	m_Panels.push_back(m_ImportPanel.get());
	m_Panels.push_back(m_ShaderPanel.get());
	m_Panels.push_back(m_EnvPanel.get());

	m_Panels.push_back(m_AudioPanel.get());

	m_Settings.push_back(m_ProjectPanel.get());
	m_Settings.push_back(m_About.get());

	// Test
	//Wiwa::EntityManager &em = m_EditorScene->GetEntityManager();
	//CreateEntityWithModelHierarchy("models/street2");
	// SceneId scene = Wiwa::SceneManager::CreateScene();//Wiwa::SceneManager::LoadScene("Assets/Scenes/SampleScene.wiscene");
	// Wiwa::SceneManager::SetScene(scene);
	/*Wiwa::Vector2i pos = Wiwa::Vector2i{0, 0};
	Wiwa::Size2i size = Wiwa::Size2i{20, 20};*/
	//uint32_t id = Wiwa::Application::Get().GetRenderer2D().CreateInstancedQuadTex(stopId, pos, size, Wiwa::Renderer2D::Pivot::CENTER);

	/*for (size_t i = 0; i < children_size; i++) {
		const Wiwa::ModelHierarchy* child_h = model_h->children[i];

		EntityId e_child = em.CreateEntity(child_h->name.c_str(), e_root);
		em.AddComponent(e_child, t3d);

		size_t c_mesh_size = child_h->meshIndexes.size();
		size_t c_child_size = child_h->children.size();

		if (child_h->meshIndexes.size() > 0) {
			mesh.modelIndex = child_h->meshIndexes[0];
			em.AddComponent(e_child, mesh);
			em.ApplySystem<Wiwa::MeshRenderer>(e_child);
		}
	}*/

	m_EventCallback = {&Wiwa::Application::OnEvent, &Wiwa::Application::Get()};

	LoadCallback();

	WI_TRACE("Editor layer attached!");
}

void EditorLayer::OnDetach()
{
	s_Instance = nullptr;
	m_Panels.clear();
	m_Settings.clear();
}

void EditorLayer::OnUpdate()
{
	if (m_ReloadLayout) {
		ImGui::LoadIniSettingsFromDisk(("config/layouts/"+m_ActiveLayout).c_str());
		m_ReloadLayout = false;
	}

	for (auto &p : m_Panels)
	{
		if (p->active)
			p->Update();
	}
	for (auto &p : m_Settings)
	{
		if (p->active)
			p->Update();
	}
}

void EditorLayer::OnImGuiRender()
{
	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);

	MainMenuBar();
	DockSpace();
	for (auto &p : m_Panels)
	{
		if (p->active)
			p->Draw();
	}
	for (auto &p : m_Settings)
	{
		if (p->active)
			p->Draw();
	}
	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);
}

void EditorLayer::OnEvent(Wiwa::Event &e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Wiwa::KeyPressedEvent>({&EditorLayer::OnKeyPressed, this});
	dispatcher.Dispatch<Wiwa::OnLoadEvent>({&EditorLayer::EditorLayer::OnLoad, this});
	dispatcher.Dispatch<Wiwa::OnSaveEvent>({&EditorLayer::EditorLayer::OnSave, this});
	dispatcher.Dispatch<Wiwa::WindowCloseEvent>({&EditorLayer::EditorLayer::OnWindowClose, this});

	for (auto it = m_Panels.end(); it != m_Panels.begin();)
	{
		(*--it)->OnEvent(e);
		if (e.Handled)
			break;
	}
}

void EditorLayer::LoadScene(const std::string& m_Path)
{
	// Unload editor scene before loading new scene into editor
	//Wiwa::SceneManager::UnloadScene(m_EditorSceneId);

	// Load scene and prepare it
	SceneId id = Wiwa::SceneManager::LoadScene(m_Path.c_str());
	Wiwa::Scene* scene = Wiwa::SceneManager::getScene(id);
	scene->GetEntityManager().AddSystemToWhitelist(FNV1A_HASH("MeshRenderer"));
	Wiwa::SceneManager::SetScene(id);

	// Update editor scene references
	m_OpenedScenePath = m_Path;
	m_EditorSceneId = id;
	m_EditorScene = scene;
}


static bool threadExec = false;
static bool finishedThread = false;
static std::mutex mutex;

void EditorLayer::RegenSolutionThread()
{
	std::string call = "call tools\\generatesol.bat ";
	call += s_SolVersion;
	call += " AppAssembly.sln ";
	call += s_BuildConf;
	system(call.c_str());
	mutex.lock();
	finishedThread = true;
	mutex.unlock();
}

void EditorLayer::RegenSol()
{
	if (threadExec)
	{
		mutex.lock();
		if (finishedThread)
		{
			s_RegenThread->join();
		}
		mutex.unlock();
		delete s_RegenThread;
	}

	s_RegenThread = new std::thread(RegenSolutionThread);
	threadExec = true;
	finishedThread = false;
}

void EditorLayer::SubmitToMainThread(const std::function<void()> func)
{
	std::scoped_lock<std::mutex> lock(m_EditorThreadMutex);

	m_EditorThreadQueue.emplace_back(func);
}

void EditorLayer::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				NewProject();
			}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				OpenProject();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				SaveProject();
			}
			if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
			{
				SaveProjectAs();
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FK_FILE " New scene", ""))
			{
				Wiwa::SceneManager::UnloadScene(m_EditorSceneId);

				m_EditorSceneId = Wiwa::SceneManager::CreateScene();
				m_EditorScene = Wiwa::SceneManager::getScene(m_EditorSceneId);
				m_OpenedScenePath = "";

				Wiwa::SceneManager::SetScene(m_EditorSceneId);
				m_EditorScene->GetEntityManager().AddSystemToWhitelist(FNV1A_HASH("MeshRenderer"));
			}
			if (ImGui::MenuItem(ICON_FK_FOLDER " Open scene", ""))
			{
				OpenScene();
			}
			if (ImGui::MenuItem(ICON_FK_FLOPPY_O " Save scene", ""))
			{
				SaveScene();
			}
			if (ImGui::MenuItem(ICON_FK_FILES_O " Save scene as..."))
			{
				SaveSceneAs();
			}
			if (ImGui::MenuItem(ICON_FK_TIMES " Close", "ALT + Q"))
				Wiwa::Application::Get().Quit();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Project settings"))
			{
				m_ProjectPanel->SwitchActive();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			int i = 1;
			for (auto &p : m_Panels)
			{
				if (ImGui::MenuItem(p->GetName(), "", p->active))
					p->SwitchActive();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Layout")) {
			if (ImGui::MenuItem("Scene layout")) {
				m_ActiveLayout = "scene_layout.ini";
				m_ReloadLayout = true;
			}
			if (ImGui::MenuItem("UI layout")) {
				
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Script"))
		{
			if (ImGui::MenuItem("Reload assembly", "ALT + R"))
				Wiwa::ScriptEngine::ReloadAssembly();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("GuiDemo", "", m_ShowDemo))
				m_ShowDemo = !m_ShowDemo;

			if (ImGui::MenuItem("Documentation"))
				Wiwa::Application::Get().OpenDir("https://github.com/ZaroDev/WiwaEngine/wiki");

			if (ImGui::MenuItem(ICON_FK_GITHUB " Download Latest"))
				Wiwa::Application::Get().OpenDir("https://github.com/ZaroDev/WiwaEngine/releases");

			if (ImGui::MenuItem("Report a bug"))
				Wiwa::Application::Get().OpenDir("https://github.com/ZaroDev/WiwaEngine/issues");

			if (ImGui::MenuItem("About", "", m_About->active))
				m_About->SwitchActive();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGuiViewportP *viewport = (ImGuiViewportP *)(void *)ImGui::GetMainViewport();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = ImGui::GetFrameHeight();

	if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
			if (ImGui::Button(ICON_FK_MOUSE_POINTER))
				m_GizmoType = -1;

			if (ImGui::Button(ICON_FK_ARROWS))
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

			if (ImGui::Button(ICON_FK_REPEAT))
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;

			if (ImGui::Button(ICON_FK_EXPAND))
				m_GizmoType = ImGuizmo::OPERATION::SCALE;

			ImGui::SetCursorPosX(Wiwa::Application::Get().GetWindow().GetWidth() / 2 - 15.0f);
			const char* play = Wiwa::Time::IsPlaying() ? ICON_FK_STOP : ICON_FK_PLAY;

			bool is_playing = Wiwa::Time::IsPlaying();

			// Play button
			if (ImGui::Button(play))
			{
				if (!is_playing) {
					SaveScene();

					if (m_OpenedScenePath != "") {
						Wiwa::Time::Play();
						Wiwa::Time::Update();

						m_SimulationSceneId = Wiwa::SceneManager::LoadScene(m_OpenedScenePath.c_str(), Wiwa::SceneManager::LOAD_SEPARATE);

						Wiwa::Scene* sc = Wiwa::SceneManager::getScene(m_SimulationSceneId);
						sc->GetEntityManager().AddSystemToWhitelist<Wiwa::MeshRenderer>();
						Wiwa::SceneManager::SetScene(m_SimulationSceneId);

						Wiwa::SceneManager::Awake();
						Wiwa::SceneManager::Init();

						Wiwa::SceneManager::PlayScene();
					}
				}
				else {
					Wiwa::Time::Stop();
					Wiwa::SceneManager::StopScene();

					// Unload simulated scene but keep resources for the editor
					Wiwa::SceneManager::UnloadScene(m_SimulationSceneId, false);

					Wiwa::SceneManager::SetScene(m_EditorSceneId);

					Audio::StopAllEvents();
				}
			}

			if (ImGui::Button(ICON_FK_PAUSE))
			{
				if (is_playing) {
					Wiwa::Time::PauseUnPause();

					if (Wiwa::SceneManager::IsPlaying()) {
						Wiwa::SceneManager::StopScene();
					}
					else {
						Wiwa::SceneManager::PlayScene();
					}
				}
			}

			if (ImGui::Button(ICON_FK_STEP_FORWARD))
			{
				if (is_playing) {
					Wiwa::Time::Step();
				}
			}

			ImGui::PopStyleColor();
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Assets browser"))
			{
				OpenCloseAssetsFolder();
			}

			ImGuiLog log = Wiwa::Application::Get().GetImGuiLayer().GetConsole();

			ImGui::PushStyleColor(ImGuiCol_Button, {0.0, 0, 0, 0});
			ImGui::AlignTextToFramePadding();

			char buff[16];
			sprintf_s(buff, 16, "%i", log.infoCount);
			ImGui::Text(buff);
			ImGui::Text(ICON_FK_INFO_CIRCLE);
			
			sprintf_s(buff, 16, "%i", log.warnCount);
			ImGui::Text(buff);
			ImGui::Text(ICON_FK_EXCLAMATION_TRIANGLE);
		
			sprintf_s(buff, 16, "%i", log.errorCount);
			ImGui::Text(buff);
			ImGui::PopStyleColor();
			ImGui::Text(ICON_FK_EXCLAMATION_CIRCLE);

			/*const char *beg = log.Buf.begin() + log.LineOffsets[log.LineOffsets.Size - 2];
			ImGui::TextUnformatted(beg, log.Buf.end());*/

			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
}

void EditorLayer::OpenCloseAssetsFolder()
{
	m_Assets->active = !m_Assets->active;
}

void EditorLayer::SaveProject()
{
	if (!Wiwa::ProjectManager::SaveProject()) {
		SaveProjectAs();
	}
}

void EditorLayer::SaveProjectAs()
{
	std::string filePath = Wiwa::FileDialogs::SaveFile("Wiwa Project (*.wiproject)\0*.wiproject\0");
	if (!filePath.empty())
	{
		Wiwa::ProjectManager::SaveProjectAs(filePath.c_str());
		WI_INFO("Succesfully saved project at path {0}", filePath.c_str());
	}
}

void EditorLayer::OpenProject()
{
	std::string filePath = Wiwa::FileDialogs::OpenFile("Wiwa Project (*.wiproject)\0*.wiproject\0");
	if (!filePath.empty())
	{
		Wiwa::ProjectManager::OpenProject(filePath.c_str());
		WI_INFO("Succesfully opened project at path {0}", filePath.c_str());
	}
}

void EditorLayer::NewProject()
{
	std::string filePath = Wiwa::FileDialogs::SaveFile("Wiwa Project (*.wiproject)\0*.wiproject\0");
	if (!filePath.empty())
	{
		Wiwa::ProjectManager::CreateProject(filePath.c_str());
		WI_INFO("Succesfully created project at path {0}", filePath.c_str());
	}
}

void EditorLayer::SaveSceneAs()
{
	std::string filePath = Wiwa::FileDialogs::SaveFile("Wiwa Scene (*.wiscene)\0*.wiscene\0");
	if (!filePath.empty())
	{
		if (filePath.find(".wiscene") == filePath.npos)
		{
			filePath += ".wiscene";
		}
		Wiwa::SceneManager::SaveScene(Wiwa::SceneManager::getActiveSceneId(), filePath.c_str());
		m_OpenedScenePath = filePath;
		WI_INFO("Succesfully saved scene at path {0}", filePath.c_str());
	}
}

void EditorLayer::SaveScene()
{
	if (m_OpenedScenePath.empty()) {
		SaveSceneAs();
	}
	else {
		Wiwa::SceneManager::SaveScene(Wiwa::SceneManager::getActiveSceneId(), m_OpenedScenePath.c_str());
	}
}

void EditorLayer::OpenScene()
{
	std::string filePath = Wiwa::FileDialogs::OpenFile("Wiwa Scene (*.wiscene)\0*.wiscene\0");
	if (!filePath.empty())
	{
		LoadScene(filePath);

		WI_INFO("Succesfully opened scene at path {0}", filePath.c_str());
	}
}

void EditorLayer::DockSpace()
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground;

	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	static bool show = false;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &show, window_flags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);
	// Submit the DockSpace
	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}

void EditorLayer::LoadPanelConfig()
{
	Wiwa::JSONDocument config("config/panels.json");

	size_t psize = m_Panels.size();

	if (config.HasMember("sol_version"))
		s_SolVersion = config["sol_version"].get<const char*>();
	if (config.HasMember("build_conf"))
		s_SolVersion = config["build_conf"].get<const char*>();
	if (config.HasMember("active_layout")) {
		m_ActiveLayout = config["active_layout"].as_string();

		ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
		ImGui::SetCurrentContext(ctx);
		ImGui::LoadIniSettingsFromDisk(("config/layouts/" + m_ActiveLayout).c_str());
	}


	for (size_t i = 0; i < psize; i++)
	{
		const auto &p = m_Panels[i];

		if (config.HasMember(p->GetName()))
		{
			p->active = config[p->GetName()];
		}
	}
}

void EditorLayer::SavePanelConfig()
{
	Wiwa::JSONDocument config;

	size_t psize = m_Panels.size();

	config.AddMember("sol_version", s_SolVersion.c_str());
	config.AddMember("build_conf", s_BuildConf.c_str());
	config.AddMember("active_layout", m_ActiveLayout.c_str());

	for (size_t i = 0; i < psize; i++)
	{
		const auto &p = m_Panels[i];

		config.AddMember(p->GetName(), p->active);
	}

	config.save_file("config/panels.json");
}

void EditorLayer::LoadCallback()
{
	Wiwa::OnLoadEvent event;
	m_EventCallback(event);
}

void EditorLayer::SaveCallback()
{
	Wiwa::OnSaveEvent event;
	m_EventCallback(event);
}

bool EditorLayer::OnKeyPressed(Wiwa::KeyPressedEvent &e)
{
	// Shortcuts
	if (e.IsRepeat())
		return false;

	bool control = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftControl) || Wiwa::Input::IsKeyPressed(Wiwa::Key::RightControl);
	bool shift = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftShift) || Wiwa::Input::IsKeyPressed(Wiwa::Key::RightShift);
	bool alt = Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftAlt) || Wiwa::Input::IsKeyPressed(Wiwa::Key::RightAlt);
	if (Wiwa::Input::IsMouseButtonPressed(1) || Wiwa::Input::IsMouseButtonPressed(0))
		return false;
	switch (e.GetKeyCode())
	{
	case Wiwa::Key::N:
	{
		if (control)
		{
			NewProject();
		} // NewScene();

		break;
	}
	case Wiwa::Key::O:
	{
		if (control)
		{
			OpenProject();
		} // OpenScene();

		break;
	}
	case Wiwa::Key::S:
	{
		if (control)
		{
			if (shift)
				SaveProjectAs();
			else
				Wiwa::ProjectManager::SaveProject();
		}

		break;
	}

	// Scene Commands
	case Wiwa::Key::D:
	{
		if (control)
		{

		} // OnDuplicateEntity();

		break;
	}

	// Gizmos
	case Wiwa::Key::Q:
	{
		if (alt)
			Wiwa::Application::Get().Quit();
		if (!ImGuizmo::IsUsing())
		{
			m_GizmoType = -1;
		}
		break;
	}
	case Wiwa::Key::W:
	{
		if (Wiwa::Input::IsMouseButtonPressed(1))
		{
			break;
		}
		if (!ImGuizmo::IsUsing())
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		break;
	}
	case Wiwa::Key::E:
	{
		if (!ImGuizmo::IsUsing())
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		break;
	}
	case Wiwa::Key::R:
	{
		if (!ImGuizmo::IsUsing())
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		break;
	}
	case Wiwa::Key::Space:
	{
		if (control)
			OpenCloseAssetsFolder();
	}
	}
	return false;
}

bool EditorLayer::OnLoad(Wiwa::OnLoadEvent &e)
{
	LoadPanelConfig();

	return false;
}

bool EditorLayer::OnSave(Wiwa::OnSaveEvent &e)
{
	SavePanelConfig();

	return false;
}

bool EditorLayer::OnWindowClose(Wiwa::WindowCloseEvent &e)
{
	SaveCallback();
	return false;
}

void EditorLayer::ExecuteMainThreadQueue()
{
	for (auto& func : m_EditorThreadQueue)
		func();

	m_EditorThreadQueue.clear();
}

