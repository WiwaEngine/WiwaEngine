#pragma once
#include "resources_impl.h"
#include "image_impl_inl.h"

namespace Wiwa {
	//--SPECIALIZATION FOR MATERIAL
	template<>
	inline void Resources::LoadMeta<Material>(const char* file)
	{

	}
	template<>
	inline void Resources::CreateMeta<Material>(const char* file)
	{
		std::filesystem::path filePath = file;
		if (!std::filesystem::exists(filePath))
			return;
		filePath += ".meta";
		JSONDocument doc;
		std::time_t time = to_time_t(std::filesystem::last_write_time(file));
		doc.AddMember("timeCreated", time);
		doc.save_file(filePath.string().c_str());
	}
	template<>
	inline ResourceId Resources::Load<Material>(const char* file)
	{
		if (!std::filesystem::exists(file))
		{
			return -1;
		}
		std::filesystem::path library_file = _assetToLibPath(file);

		std::string file_path = library_file.string();
		standarizePath(file_path);

		ResourceId position = getResourcePosition(WRT_MATERIAL, file_path.c_str());
		size_t size = m_Resources[WRT_MATERIAL].size();

		ResourceId resourceId;

		if (position == size) {

			Material* material = new Material(file_path.c_str());

			PushResource(WRT_MATERIAL, file_path.c_str(), material);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Material* Resources::GetResourceById<Material>(ResourceId id)
	{
		Material* material = NULL;

		if (id >= 0 && id < m_Resources[WRT_MATERIAL].size()) {
			material = static_cast<Material*>(m_Resources[WRT_MATERIAL][id]->resource);
		}

		return material;
	}
	template<>
	inline bool Resources::Import<Material>(const char* file)
	{
		if (!_file_exists(file)) return false;

		std::filesystem::path import_file = file;
		std::filesystem::path export_file = _assetToLibPath(file);
		export_file.replace_extension(".wimaterial");

		std::filesystem::path export_path = export_file.parent_path();

		if (!_preparePath(export_path.string())) return false;

		Material* mat = Material::LoadMaterialData(file);
		std::vector<Uniform>& uniforms = mat->getUniforms();
		size_t size = uniforms.size();

		for (size_t i = 0; i < size; i++) {
			if (uniforms[i].getType() == UniformType::Sampler2D) {
				Uniform::SamplerData* sdata = uniforms[i].getPtrData<Uniform::SamplerData>();

				if (!Resources::Import<Image>(sdata->tex_path.c_str())) {
					return false;
				}

				sdata->tex_path = _assetToLibPath(sdata->tex_path);

				//uniforms[i].setData(*sdata, UniformType::Sampler2D);
			}
		}

		mat->SaveLib(export_file.string().c_str());

		delete mat;

		WI_CORE_INFO("Material at {} imported succesfully!", import_file.string().c_str());
		return true;
	}
	template<>
	inline bool Resources::CheckImport<Material>(const char* file)
	{
		return _check_import_impl(file, ".wimaterial");
	}
	template<>
	inline const char* Resources::getResourcePathById<Material>(size_t id)
	{
		return getPathById(WRT_MATERIAL, id);
	}
}