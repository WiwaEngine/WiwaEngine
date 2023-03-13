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
#include <Wiwa/ecs/components/ParticleEmitter.h>


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
		if (type->hash == (size_t)TypeHash::ParticleEmitter) { DrawParticleEmitterComponent(data); }else
			
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

	//ImGui::Checkbox("Is default listener", &asrc->isDefaultListener);
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

void ParticleTab()
{
	ImGui::Dummy(ImVec2(38, 0));
	ImGui::SameLine();
}

void InspectorPanel::DrawParticleEmitterComponent(byte* data)
{

	Wiwa::ParticleEmitter* emitter = (Wiwa::ParticleEmitter*)data;

	ImGui::Separator();
	ImGui::Text("Emitter Parameters");
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Checkbox("##repeat", &emitter->repeat);
	ImGui::SameLine();
	ImGui::Text("Loop Spawning");

	//spawn rate
	if (emitter->repeat)
	{
		ParticleTab();
		ImGui::Checkbox("##particle_rate_isRanged", &emitter->particle_rate_isRanged);
		ImGui::SameLine();




		if (emitter->particle_rate_isRanged)
		{
			ImGui::Text("Spawn Rate Range");

			ParticleTab();
			ImGui::PushItemWidth(46.0f);

			ParticleTab();

			ImGui::DragFloat("##particle_rate_range0", &emitter->particle_rate_range[0], 0.01f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();


			ImGui::DragFloat("##particle_rate_range1", &emitter->particle_rate_range[1], 0.01f, 0.0f, 0.0f, "%.2f");
		}
		else
		{
			ImGui::Text("Spawn Rate");

			ParticleTab();

			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);


			ImGui::DragFloat("##particle_rate", &emitter->particle_rate, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopItemWidth();

		if (emitter->particle_rate < 0)		emitter->particle_rate = 0.01f;
		if (emitter->particle_rate_range[0] < 0)		emitter->particle_rate_range[0] = 0.01f;
		if (emitter->particle_rate_range[1] < 0)		emitter->particle_rate_range[1] = 0.01f;
	}

	
	//spawn amount
	{
		ImGui::Checkbox("##particle_amount_isRanged", &emitter->particle_amount_isRanged);
		ImGui::SameLine();


		if (emitter->particle_amount_isRanged)
		{
			ImGui::Text("Spawn Amount Range");

			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();


			ImGui::PushItemWidth(46.0f);


			ImGui::DragInt("##particle_amount_range0", &emitter->particle_amount_range[0], 0.05f, 0.0f, 0.0f, "%.2f");

			ImGui::SameLine();


			ImGui::DragInt("##particle_amount_range1", &emitter->particle_amount_range[1], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{
			ImGui::Text("Spawn Amount");

			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);


			ImGui::DragInt("##particle_amount", &emitter->particle_amount, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopItemWidth();

		if (emitter->particle_amount < 0)		emitter->particle_amount = 0;
		if (emitter->particle_amount_range[0] < 0)		emitter->particle_amount_range[0] = 0;
		if (emitter->particle_amount_range[1] < 0)		emitter->particle_amount_range[1] = 0;
	}



	//particle attributes

	ImGui::Separator();
	ImGui::Text("Particle attributes");
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 4));

	//particle follow emitter
	{
		ImGui::Dummy(ImVec2(0, 0));
		ImGui::SameLine();

		ImGui::Checkbox("##particle_followEmitter", &emitter->particle_followEmitter);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(2.5, 0));
		ImGui::SameLine();
		ImGui::Text("Follow Emitter");
	}

	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Text("Range");

	ImGui::Dummy(ImVec2(0, 4));


	


	//particle lifetime
	{
		ImGui::Dummy(ImVec2(0, 0));
		ImGui::SameLine();


		ImGui::Checkbox("##particle_lifetime_isRanged", &emitter->particle_lifetime_isRanged);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(2, 0));
		ImGui::SameLine();
		ImGui::SameLine();
		ImGui::Text("Particle life time");

		if (emitter->particle_lifetime_isRanged)
		{
			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(46.0f);


			ImGui::DragFloat("##particle_lifetime_range0", &emitter->particle_lifetime_range[0], 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();


			ImGui::DragFloat("##particle_lifetime_range1", &emitter->particle_lifetime_range[1], 0.05f, 0.0f, 0.0f, "%.2f");

		}
		else
		{
			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);
			ImGui::DragFloat("##particle_lifetime", &emitter->particle_lifetime, 0.05f, 0.0f, 0.0f, "%.2f");

		}
		ImGui::PopItemWidth();

		if (emitter->particle_lifetime < 0) emitter->particle_lifetime = 0;
		if (emitter->particle_lifetime_range[0] < 0) emitter->particle_lifetime_range[0] = 0;
		if (emitter->particle_lifetime_range[1] < 0) emitter->particle_lifetime_range[1] = 0;
	}


	if (ImGui::TreeNode("Position & Translation"))
	{
		//particle starting position
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_startingPosition_isRanged", &emitter->particle_startingPosition_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::Text("Particle Starting Position");

			if (emitter->particle_startingPosition_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingPosition_range0", &(emitter->particle_startingPosition_range[0])[0], 0.05f, 0.0f, 0.0f, "%.2f");
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingPosition_range1", &(emitter->particle_startingPosition_range[1])[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingPosition", &(emitter->particle_startingPosition)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}


		}

		//particle initial velocity
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_velocity_isRanged", &emitter->particle_velocity_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Velocity");
			if (emitter->particle_velocity_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_velocity_range0", &(emitter->particle_velocity_range[0])[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*	ImGui::SameLine();
					ImGui::Text("Particle Velocity");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_velocity_range1", &(emitter->particle_velocity_range[1])[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_velocity", &(emitter->particle_velocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Velocity");*/
			}
		}

		//particle constant acceleration
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_acceleration_isRanged", &emitter->particle_acceleration_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Acceleration");
			if (emitter->particle_acceleration_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_acceleration_range0", &(emitter->particle_acceleration_range[0])[0], 0.01f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Acceleration");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_acceleration_range1", &(emitter->particle_acceleration_range[1])[0], 0.01f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_acceleration", &(emitter->particle_acceleration)[0], 0.01f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Acceleration");*/
			}
		}

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("Rotation"))
	{
		//particle starting rotation
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_startingRotation_isRanged", &emitter->particle_startingRotation_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::Text("Particle Rotation");

			if (emitter->particle_startingRotation_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingRotation_range0", &(emitter->particle_startingRotation_range[0])[0], 0.05f, 0.0f, 0.0f, "%.2f");
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingRotation_range1", &(emitter->particle_startingRotation_range[1])[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingRotation", &(emitter->particle_startingRotation)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Size & Growth"))
	{
		//particle starting size
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_startingSize_isRanged", &emitter->particle_startingSize_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Starting Size");
			if (emitter->particle_startingSize_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingSize_range0", &(emitter->particle_startingSize_range[0])[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingSize_range1", &(emitter->particle_startingSize_range[1])[0], 0.05f, 1.0f, 1.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingSize", &emitter->particle_startingSize[0], 0.05f, 0.0f, 10.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
			}
		}

		//particle size constant growth rate
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_growthVelocity_isRanged", &emitter->particle_growthVelocity_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Growth Rate");
			if (emitter->particle_growthVelocity_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_growthVelocity_range0", &(emitter->particle_growthVelocity_range[0])[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_growthVelocity_range1", &(emitter->particle_growthVelocity_range[1])[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_growthVelocity", &emitter->particle_growthVelocity[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Texture"))
	{
		//particle starting size
		{
			/*ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_texture", &emitter->particle_startingSize_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();*/

			ImGui::Text("Particle Texture");

			uint32_t image_id = 0;

			if (emitter->texture)
				image_id = emitter->texture->GetTextureId();

			ImGui::Image(ImTextureID(image_id), { 128,128 });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{


					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png" || p.extension() == ".jpg")
					{
						WI_CORE_INFO("test");

						bool importedCorrectly = Wiwa::Resources::CheckImport<Wiwa::Image>(pathS.c_str());

						emitter->textId1 = Wiwa::Resources::Load<Wiwa::Image>(pathS.c_str());

						if (emitter->textId1 == WI_INVALID_INDEX)
						{
							WI_CORE_INFO("Error loading Image: [WI_INVALID_INDEX]");

						}
						else
						{
							emitter->texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(emitter->textId1);

							/*std::string test = "texture id: " + std::to_string(emitter->texture->GetTextureId());

							WI_CORE_INFO(test);*/


							

						}

						/*WI_INFO("Trying to load payload at path {0}", pathS.c_str());
						p.replace_extension();
						std::string src = Wiwa::FileSystem::RemoveFolderFromPath("assets", p.string());
						mesh->meshId = Wiwa::Resources::Load<Wiwa::Model>(src.c_str());
						mesh->modelIndex = 0;
						mesh->drawChildren = true;

						Wiwa::Model* m = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);*/
					}
				}

				ImGui::EndDragDropTarget();
			}
		}

		ImGui::TreePop();
	}
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
