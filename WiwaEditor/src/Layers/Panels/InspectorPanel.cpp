#include "InspectorPanel.h"

#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

#include <imgui_internal.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/math/Vector2i.h>
#include "../../Utils/EditorUtils.h"

#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/ecs/components/AudioSource.h>
#include <Wiwa/ecs/components/PointLight.h>
#include <Wiwa/ecs/components/SpotLight.h>
#include <Wiwa/ecs/components/DirectionalLight.h>

bool InspectorPanel::DrawComponent(size_t componentId)
{
	bool ret = true;

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	const Type* type = em.GetComponentType(componentId);

	std::string name = type->name;

	if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string del_label = "Delete##comp";
		del_label += name.c_str();
		if (!(type->hash == (size_t)TypeHash::Transform3D))
		{
			if (ImGui::Button(del_label.c_str()))
			{
				ret = false;
			}
		}

		byte* data = em.GetComponent(m_CurrentID, componentId, type->size);

		// Custom component interface
		if (type->hash == (size_t)TypeHash::Mesh) {	DrawMeshComponent(data); } else
		if (type->hash == (size_t)TypeHash::Transform3D) { DrawTransform3dComponent(data); } else
		if (type->hash == (size_t)TypeHash::AudioSource) { DrawAudioSourceComponent(data); } else
		if (type->hash == (size_t)TypeHash::PointLight) { DrawPointLightComponent(data); } else
		if (type->hash == (size_t)TypeHash::DirectionalLight) { DrawDirectionalLightComponent(data); } else
		if (type->hash == (size_t)TypeHash::SpotLight) { DrawSpotLightComponent(data); } else
		if (type->hash == (size_t)TypeHash::Rigidbody) { DrawRigidBodyComponent(data); } else
		if (type->hash == (size_t)TypeHash::ColliderCube) { DrawColliderCubeComponent(data); } else
		if (type->hash == (size_t)TypeHash::ColliderSphere) { DrawColliderSpehereComponent(data); } else
		if (type->hash == (size_t)TypeHash::ColliderCylinder) { DrawColliderCylinderComponent(data); } else
		if (type->hash == (size_t)TypeHash::RayCast) { DrawRayCastComponent(data); } else
			
		// Basic component interface
		if (type->is_class) {
			const Class* cl = (const Class*)type;

			for (size_t i = 0; i < cl->fields.size(); i++)
			{
				DrawField(data, cl->fields[i]);
			}
		}
		ImGui::TreePop();
	}
	
	return ret;
}

void InspectorPanel::DrawField(unsigned char* data, const Field& field)
{
	// Draw class field
	if (field.type->is_class) {
		const Class* cl = (const Class*)field.type;

		std::string name = field.name;

		if (ImGui::TreeNodeEx(name.c_str()))
		{
			for (size_t i = 0; i < cl->fields.size(); i++)
			{
				DrawField(data + field.offset, cl->fields[i]);
			}
			ImGui::TreePop();
		}
		return;
	}

	// Draw enum field
	if (field.type->is_enum) {
		const Enum* en = (const Enum*)field.type;

		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());

		//TODO: DRAW LIST OF ENUMS TO CHOOSE FROM

		ImGui::PopID();
		return;
	}

	// Draw basic fields
	if (field.type->hash == (size_t)TypeHash::Vector2i)
	{
		ImGui::PushID(field.name.c_str());
		DrawInt2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Float)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputFloat("", (float*)(data + field.offset));
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Vector2f)
	{
		ImGui::PushID(field.name.c_str());
		DrawVec2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Vector3f)
	{
		ImGui::PushID(field.name.c_str());
		DrawVec3Control(field.name.c_str(), data, field,0.0f);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::UInt64)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Int32)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Rect2i)
	{
		ImGui::PushID(field.name.c_str());
		DrawRect2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Pivot)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int*)(data + field.offset));
		ImGui::PopID();
	}
}

void InspectorPanel::DrawCollisionTags()
{
	Wiwa::PhysicsManager& py = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

	if (ImGui::Button("Edit Tags"))
		ImGui::OpenPopup("Edit Tags");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Edit Tags", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static int selected = -1;
		for (int n = 0; n < py.filterStrings.size(); n++)
		{
			std::string tempStr = py.filterStrings[n].c_str();
			tempStr += "_" + py.fliterBitsSets[n].to_string();
			if (ImGui::Selectable(tempStr.c_str(), selected == n, ImGuiSelectableFlags_DontClosePopups))
				selected = n;
		}

		static char strBuf[64] = ""; ImGui::InputText("Write Tag", strBuf, 64, ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

		if (ImGui::Button("Add", ImVec2(120, 0)))
		{
			if (!(std::find(py.filterStrings.begin(), py.filterStrings.end(), strBuf) != py.filterStrings.end()))
			{
				py.AddFilterTag(strBuf);
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Remove", ImVec2(120, 0)))
		{
			// If only index > 0, because we don't want to erase the basic tag
			if (selected > 0)
				py.RemoveFilterTag(selected);
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
		// stored in the object itself, etc.)
	//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
	//static int itemCurrentIdx = 0; // Here we store our selection data as an index.
	//const char* comboPreviewValue = py.filterStrings[itemCurrentIdx].c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
	//if (ImGui::BeginCombo("Collision Tags", comboPreviewValue))
	//{
	//	for (int n = 0; n < py.filterStrings.size(); n++)
	//	{
	//		const bool is_selected = (itemCurrentIdx == n);
	//		if (ImGui::Selectable(py.filterStrings[n].c_str(), is_selected))
	//			itemCurrentIdx = n;

	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}

	//	

	//	ImGui::EndCombo();
	//}
}

void InspectorPanel::DrawMeshComponent(byte* data)
{
	Wiwa::Mesh* mesh = (Wiwa::Mesh*)data;

	// Draw meshId field
	ImGui::Text("Model");
	ImGui::PushID("meshId");


	Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
	mod = mod->getModelAt(mesh->modelIndex);
	AssetContainer(std::filesystem::path(mod->getModelPath()).stem().string().c_str());
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::wstring ws(path);
			std::string pathS(ws.begin(), ws.end());
			std::filesystem::path p = pathS.c_str();
			if (p.extension() == ".fbx" || p.extension() == ".FBX")
			{
				WI_INFO("Trying to load payload at path {0}", pathS.c_str());
				p.replace_extension();
				std::string src = Wiwa::FileSystem::RemoveFolderFromPath("assets", p.string());
				mesh->meshId = Wiwa::Resources::Load<Wiwa::Model>(src.c_str());
				mesh->modelIndex = 0;
				mesh->drawChildren = true;

				Wiwa::Model* m = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Text("Model path: ");
	ImGui::SameLine();
	ImGui::Text(mod->getModelPath());
	ImGui::Text("Bounding box");
	ImGui::InputFloat3("Max", (float*)glm::value_ptr(mod->boundingBox.getMax()));
	ImGui::InputFloat3("Min", (float*)glm::value_ptr(mod->boundingBox.getMin()));
	static bool showNormals = false;
	if (ImGui::Checkbox("Show normals", &showNormals))
		mod->showNormals = showNormals;
	ImGui::PopID();

	//Draw materialId field
	ImGui::Text("Material");
	Wiwa::Material* mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(mesh->materialId);
	AssetContainer(std::filesystem::path(mat->getMaterialPath()).stem().string().c_str());
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::wstring ws(path);
			std::string pathS(ws.begin(), ws.end());
			std::filesystem::path p = pathS.c_str();
			if (p.extension() == ".wimaterial")
			{
				WI_INFO("Trying to load payload at path {0}", pathS.c_str());
				mesh->materialId = Wiwa::Resources::Load<Wiwa::Material>(pathS.c_str());
			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::PushID("materialId");
	ImGui::Text("Material at: ");
	ImGui::SameLine();
	ImGui::Text(mat->getMaterialPath());
	ImGui::PopID();
}

void InspectorPanel::DrawTransform3dComponent(byte* data)
{
	Wiwa::Transform3D* transform = (Wiwa::Transform3D*)data;
	DrawVec3Control("Position", &transform->localPosition, 0.0f, 100.0f);
	DrawVec3Control("Rotation", &transform->localRotation, 0.0f, 100.0f);
	DrawVec3Control("Scale", &transform->localScale, 1.0f, 100.0f);
}

void InspectorPanel::DrawAudioSourceComponent(byte* data)
{
	Wiwa::AudioSource* asrc = (Wiwa::AudioSource*)data;

	ImGui::Checkbox("Is default listener", &asrc->isDefaultListener);
	ImGui::InputText("Event", asrc->eventName, sizeof(asrc->eventName));
	ImGui::Checkbox("Play on awake", &asrc->playOnAwake);
}

void InspectorPanel::DrawPointLightComponent(byte* data)
{
	Wiwa::PointLight* lsrc = (Wiwa::PointLight*)data;

	ImGui::ColorEdit3("Color", glm::value_ptr(lsrc->Color));
	ImGui::InputFloat("Ambient Intensity", &lsrc->AmbientIntensity, 0.05f, 0.5f, "%.2f");
	ImGui::InputFloat("Diffuse Intensity", &lsrc->DiffuseIntensity, 0.05f, 0.5f, "%.2f");
	ImGui::SliderFloat("Constant", &lsrc->Constant, 0.001f, 1.0f);
	ImGui::SliderFloat("Linear", &lsrc->Linear, 0.001f, 1.0f);
	ImGui::SliderFloat("Exponential", &lsrc->Exp, 0.001f, 1.0f);

}

void InspectorPanel::DrawDirectionalLightComponent(byte* data)
{
	Wiwa::DirectionalLight* lsrc = (Wiwa::DirectionalLight*)data;

	ImGui::ColorEdit3("Color", glm::value_ptr(lsrc->Color));
	ImGui::InputFloat("Ambient intensity", &lsrc->AmbientIntensity, 0.5f, 1.0f, "%.2f");
	ImGui::InputFloat("Diffuse intensity", &lsrc->DiffuseIntensity, 0.5f, 1.0f, "%.2f");
}
//TODO: Implement when ready
void InspectorPanel::DrawSpotLightComponent(byte* data)
{
	Wiwa::SpotLight* lsrc = (Wiwa::SpotLight*)data;

	ImGui::ColorEdit3("Color", glm::value_ptr(lsrc->Color));
	ImGui::InputFloat("Ambient Intensity", &lsrc->AmbientIntensity, 0.5f, 1.0f, "%.2f");
	ImGui::InputFloat("Diffuse Intensity", &lsrc->DiffuseIntensity, 0.5f, 1.0f, "%.2f");
	ImGui::SliderFloat("Constant", &lsrc->Constant, 0.001f, 1.0f);
	ImGui::SliderFloat("Linear", &lsrc->Linear, 0.001f, 1.0f);
	ImGui::SliderFloat("Exponential", &lsrc->Exp, 0.001f, 1.0f);
	DrawVec3Control("Direction", &lsrc->Direction);
	ImGui::InputFloat("Cutoff", &lsrc->Cutoff);
}

void InspectorPanel::DrawRigidBodyComponent(byte* data)
{
	Wiwa::PhysicsManager& py = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
	Wiwa::Rigidbody* rigidBody = (Wiwa::Rigidbody*)data;
	DrawVec3Control("Position offset", &rigidBody->positionOffset, 0.0f, 100.0f);
	DrawVec3Control("Scaling offset", &rigidBody->scalingOffset, 0.0f, 100.0f);
	ImGui::Checkbox("Is static?", &rigidBody->isStatic);
	ImGui::Checkbox("Is trigger?", &rigidBody->isTrigger);
	ImGui::Checkbox("Do continuous?", &rigidBody->doContinuousCollision);

	const char* comboPreviewValue = py.filterStrings[rigidBody->selfTag].c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo("Self Tag", comboPreviewValue))
	{
		for (int n = 0; n < py.filterStrings.size(); n++)
		{
			const bool is_selected = (rigidBody->selfTag == n);
			if (ImGui::Selectable(py.filterStrings[n].c_str(), is_selected))
				rigidBody->selfTag = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Text("Collide with:");
	for (int i = 0; i < py.filterStrings.size(); i++)
	{
		bool local = (rigidBody->filterBits >> i) & 1; //Checking a bit
		ImGui::Checkbox(py.filterStrings[i].c_str(), &local);
		if (local)
			rigidBody->filterBits |= 1 << i;
		else
			rigidBody->filterBits &= ~(1 << i);
	}

	//ImGui::Text("Do not collide with:");
	//for (int i = 0; i < py.filterStrings.size(); i++)
	//{
	//	bool local = (rigidBody->filterBits >> i) & 1U; //Checking a bit
	//	ImGui::Checkbox(py.filterStrings[i].c_str(), &local);
	//	if (local)
	//		rigidBody->filterBits |= 1UL << i;
	//	else
	//		rigidBody->filterBits &= ~(1UL << i);
	//}

}

void InspectorPanel::DrawColliderCubeComponent(byte* data)
{
	Wiwa::ColliderCube* colliderCube = (Wiwa::ColliderCube*)data;
	DrawVec3Control("HalfExtents", &colliderCube->halfExtents, 0.0f, 100.0f);
}

void InspectorPanel::DrawColliderSpehereComponent(byte* data)
{
	Wiwa::ColliderSphere* colliderSphere = (Wiwa::ColliderSphere*)data;
	ImGui::InputFloat("Radius", &colliderSphere->radius);
}

void InspectorPanel::DrawColliderCylinderComponent(byte* data)
{
	Wiwa::ColliderCylinder* colliderCylinder = (Wiwa::ColliderCylinder*)data;
	ImGui::InputFloat("Radius", &colliderCylinder->radius);
	ImGui::InputFloat("Height", &colliderCylinder->height);
}

void InspectorPanel::DrawRayCastComponent(byte* data)
{
	Wiwa::RayCast* rayCast = (Wiwa::RayCast*)data;
	DrawVec3Control("Ray From", &rayCast->rayFromWorld, 0.0f, 100.0f);
	DrawVec3Control("Ray To", &rayCast->rayToWorld, 0.0f, 100.0f);
	ImGui::Checkbox("Enabled", &rayCast->doRayCasting);
}

InspectorPanel::InspectorPanel(EditorLayer* instance)
	: Panel("Inspector", ICON_FK_INFO_CIRCLE, instance)
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Draw()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(iconName.c_str(), &active);
	if (m_EntitySet && m_CurrentID >= 0)
	{
		const char* entName = em.GetEntityName(m_CurrentID);
		std::string edit = entName;
		
		ImGui::InputText("Name", (char*)edit.c_str(), 64);
		ImGui::SameLine();
		ImGui::Text("(%i)", m_CurrentID);

		DrawCollisionTags();

		if (ImGui::Button("Delete##entity"))
		{
			m_EntitySet = false;
			em.DestroyEntity(m_CurrentID);
		}
		if (strcmp(edit.c_str(), entName) != 0)
			em.SetEntityName(m_CurrentID, edit.c_str());
		
		if(ImGui::CollapsingHeader("Components"))
			DrawComponents(em);

		if (ImGui::CollapsingHeader("Systems"))
			DrawSystems(em);
	}
	else
	{
		TextCentered("Select an entity to inspect");
	}
	ImGui::End();
}

void InspectorPanel::DrawComponents(Wiwa::EntityManager& em)
{
	std::map<ComponentId, size_t>& map = em.GetEntityComponents(m_CurrentID);
	bool removed = false;
	size_t idToRemove;

	for (std::map<ComponentId, size_t>::iterator comp = map.begin(); comp != map.end(); comp++)
	{
		if (!DrawComponent(comp->first))
		{
			idToRemove = comp->first;
			removed = true;
		}
	}

	if (removed) {
		em.RemoveComponentById(m_CurrentID, idToRemove);
	}
	
	ImGui::Separator();

	if (ButtonCenteredOnLine("Add component"))
		ImGui::OpenPopup("Components");

	size_t c_count = Wiwa::Application::Get().GetComponentTypeCount();

	if (ImGui::BeginPopup("Components"))
	{
		static ImGuiTextFilter filter;
		ImGui::Text("Search:");
		filter.Draw("##searchbar", 340.f);
		ImGui::BeginChild("listbox child", ImVec2(300, 200));
		for (size_t i = 0; i < c_count; i++) {
			const Type* type = Wiwa::Application::Get().GetComponentType(i);
			const char* paintkit = type->name.c_str();
			if (filter.PassFilter(paintkit))
			{
				std::string label = paintkit;

				label += "##" + std::to_string(i);
				if (ImGui::MenuItem(label.c_str()))
				{
					em.AddComponent(m_CurrentID, type);
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}
}

void InspectorPanel::DrawSystems(Wiwa::EntityManager& em)
{
	std::vector<SystemHash>& systems = em.GetEntitySystemHashes(m_CurrentID);

	for (size_t i = 0; i < systems.size(); i++)
	{
		ImGui::PushID(i);
		const Type* system = Wiwa::Application::Get().GetSystemTypeH(systems[i]);
		ImGui::Text(system->name.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			em.RemoveSystem(m_CurrentID, system->hash);
		}
		ImGui::PopID();
	}

	if (ButtonCenteredOnLine("Add System"))
		ImGui::OpenPopup("System");

	size_t c_count = Wiwa::Application::Get().GetSystemTypeCount();

	if (ImGui::BeginPopup("System"))
	{
		static ImGuiTextFilter filter;
		ImGui::Text("Search:");
		filter.Draw("##searchbar", 340.f);
		ImGui::BeginChild("listbox child", ImVec2(300, 200));
		for (size_t i = 0; i < c_count; i++) {
			const Type* type = Wiwa::Application::Get().GetSystemType(i);
			const char* paintkit = type->name.c_str();
			if (filter.PassFilter(paintkit))
			{
				std::string label = paintkit;

				label += "##" + std::to_string(i);
				if (ImGui::MenuItem(label.c_str()))
				{
					em.ApplySystem(m_CurrentID, type);
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}
}

void InspectorPanel::Update()
{
}

void InspectorPanel::OnEvent(Wiwa::Event&e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<EntityChangeEvent>({ &InspectorPanel::OnEntityChangeEvent, this });
	dispatcher.Dispatch<Wiwa::SceneChangeEvent>({ &InspectorPanel::OnSceneChangeEvent, this });
}

bool InspectorPanel::OnEntityChangeEvent(EntityChangeEvent& e)
{
	m_CurrentID = e.GetResourceId();
	m_EntitySet = true;
	return false;
}

bool InspectorPanel::OnSceneChangeEvent(Wiwa::SceneChangeEvent& e)
{
	m_CurrentID = -1;
	m_EntitySet = false;
	return false;
}
