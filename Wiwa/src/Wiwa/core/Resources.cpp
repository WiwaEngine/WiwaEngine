#include <wipch.h>
#include "Resources.h"

#include "../vendor/stb/stb_image.h"

namespace Wiwa {
	std::vector<Resources::Resource*> Resources::m_Resources[Resources::WRT_LAST];

	Resources::Resources()
	{
	}

	void Resources::PushResource(ResourceType rt, const char* file, void* rsc)
	{
		Resource* resource = new Resource();

		resource->filePath = file;
		resource->resource = rsc;

		m_Resources[rt].push_back(resource);

		std::string message = "Loaded resource \"" + resource->filePath + "\" successfully.";

		WI_CORE_INFO(message.c_str());
	}

	ResourceId Resources::getResourcePosition(ResourceType rt, const char * file)
	{
		size_t size = m_Resources[rt].size();

		ResourceId resourceId = size;

		for (size_t i = 0; i < size; i++) {
			if (m_Resources[rt][i]->filePath == file) {
				resourceId = i;
				break;
			}
		}

		return resourceId;
	}

	std::string Resources::_assetToLibPath(std::string path)
	{
		size_t a_ind = path.find("assets");

		path.replace(a_ind, 6, "library");

		return path;
	}

	bool Resources::_preparePath(std::string path)
	{
		if (!std::filesystem::exists(path)) {
			if (!std::filesystem::create_directories(path)) {
				WI_ERROR("Couldn't create directory {}", path.c_str());
				return false;
			}
		}

		return true;
	}

	void Resources::_import_image_impl(const char* origin, const char* destination)
	{
		int w, h, ch;

		// STBI_rgb_alpha loads image as 32 bpp (4 channels), ch = image origin channels
		unsigned char* image = stbi_load(origin, &w, &h, &ch, STBI_rgb_alpha);
		if (w != h)
		{
			WI_ERROR("Image at {0} needs to be square in order to be imported", origin);
			return;
		}
		Image::raw_to_dds_file(destination, image, w, h, 32);

		stbi_image_free(image);
	}

	void Resources::_import_model_impl(const char* origin, const char* destination)
	{
		Model* model = Model::GetModelFromFile(origin);

		Model::SaveModel(model, destination);

		delete model;
	}

	void Resources::Clear()
	{
		/*for (int i = 0; i < RT_LAST; i++) {
			size_t rsize = mResources[i].size();

			for (size_t j = 0; j < rsize; j++) {
				delete mResources[i][j]->resource;
				delete mResources[i][j];
			}
		}*/
	}
	
}