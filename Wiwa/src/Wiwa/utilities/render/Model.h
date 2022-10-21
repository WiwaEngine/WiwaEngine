#pragma once

#include <Wiwa/Core.h>

#include <vector>
#include <Wiwa/utilities/math/Math.h>

class aiMesh;

namespace Wiwa {
	class WI_API Model
	{
	protected:
		bool is_root;

		std::vector<float> vbo_data;
		std::vector<int> ebo_data;

		std::vector<Model*> models;

		void generateBuffers();
	private:
		unsigned int vao, vbo, ebo;

		void getMeshFromFile(const char* file);

		Model* loadmesh(const aiMesh* mesh);
		
		void CreateCube();
		void CreatePlane();
		void CreatePyramid();
		void CreateSphere();
	public:
		Model(const char* file);
		~Model();

		void Render();

		inline unsigned int getVAO() { return vao; }
	};
}