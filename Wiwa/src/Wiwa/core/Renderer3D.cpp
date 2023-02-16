#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/Log.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <glew.h>
#include <Wiwa/utilities/render/shaders/Shader.h>

#include <Wiwa/ecs/systems/LightSystem.h>

namespace Wiwa {
	Renderer3D::Renderer3D() {
	}

	Renderer3D::~Renderer3D()
	{
	}

	bool Renderer3D::Init()
	{
		Size2i& resolution = Application::Get().GetTargetResolution();

		WI_CORE_INFO("Renderer3D initialized");
		SetOption(Options::DEPTH_TEST);
		SetOption(Options::CULL_FACE);

		//Init default shaders with uniforms
		ResourceId textShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/light/lit_model_textured");
		Shader* textShader = Wiwa::Resources::GetResourceById<Shader>(textShaderId);
		textShader->Compile("resources/shaders/light/lit_model_textured");
		textShader->addUniform("u_Texture", UniformType::Sampler2D);
		textShader->addUniform("u_SpecularValue", UniformType::Float);
		textShader->addUniform("u_MatAmbientColor", UniformType::fVec4);
		textShader->addUniform("u_MatDiffuseColor", UniformType::fVec4);
		textShader->addUniform("u_MatSpecularColor", UniformType::fVec4);
		Wiwa::Resources::Import<Shader>("resources/shaders/light/lit_model_textured", textShader);

		ResourceId colorShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/light/lit_model_color");
		Shader* colorShader = Wiwa::Resources::GetResourceById<Shader>(colorShaderId);
		colorShader->Compile("resources/shaders/light/lit_model_color");
		colorShader->addUniform("u_Color", UniformType::fVec4);
		colorShader->addUniform("u_SpecularValue", UniformType::Float);
		colorShader->addUniform("u_MatAmbientColor", UniformType::fVec4);
		colorShader->addUniform("u_MatDiffuseColor", UniformType::fVec4);
		colorShader->addUniform("u_MatSpecularColor", UniformType::fVec4);
		Wiwa::Resources::Import<Shader>("resources/shaders/light/lit_model_color", colorShader);


		//Normal Display Shader
		m_NormalDisplayShaderId = Resources::Load<Shader>("resources/shaders/debug/normal_display");
		m_NormalDisplayShader = Resources::GetResourceById<Shader>(m_NormalDisplayShaderId);
		m_NormalDisplayShader->Compile("resources/shaders/debug/normal_display");

		m_NDSUniforms.Model = m_NormalDisplayShader->getUniformLocation("u_Model");
		m_NDSUniforms.View = m_NormalDisplayShader->getUniformLocation("u_View");
		m_NDSUniforms.Projection = m_NormalDisplayShader->getUniformLocation("u_Projection");
		//Bounding box
		m_BBDisplayShaderId = Resources::Load<Shader>("resources/shaders/debug/boundingbox_display");
		m_BBDisplayShader = Resources::GetResourceById<Shader>(m_BBDisplayShaderId);
		m_BBDisplayShader->Compile("resources/shaders/debug/boundingbox_display");
		m_BBDSUniforms.Model = m_BBDisplayShader->getUniformLocation("u_Model");
		m_BBDSUniforms.View = m_BBDisplayShader->getUniformLocation("u_View");
		m_BBDSUniforms.Projection = m_BBDisplayShader->getUniformLocation("u_Proj");


		std::vector<const char*> faces = {
			"resources/images/skybox/right.jpg",
			"resources/images/skybox/left.jpg",
			"resources/images/skybox/top.jpg",
			"resources/images/skybox/bottom.jpg",
			"resources/images/skybox/front.jpg",
			"resources/images/skybox/back.jpg"
		};
		
		m_DefaultSkybox.LoadCubemap(faces);

		return true;
	}

	void Renderer3D::Update()
	{
		RenderSkybox();
	}



	void Renderer3D::RenderMesh(Model* mesh, const Transform3D& t3d, Material* material, const size_t& directional,
		const std::vector<size_t>& pointLights, bool clear/*=false*/, Camera* camera/*=NULL*/, bool cull /*= false*/)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(t3d.position.x, t3d.position.y, t3d.position.z));
		model = glm::rotate(model, glm::radians(t3d.rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(t3d.rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(t3d.rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(t3d.scale.x, t3d.scale.y, t3d.scale.z));

		Shader* matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(model, camera->getView(), camera->getProjection());
		SetUpLight(matShader, camera, directional, pointLights);

		material->Bind();

		mesh->Render();

		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}
		
		camera->frameBuffer->Unbind();
	}


	void Renderer3D::RenderMesh(Model* mesh, const Transform3D& t3d, const Transform3D& parent, Material* material, const size_t& directional,
		const std::vector<size_t>& pointLights, bool clear, Camera* camera, bool cull)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();;
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);

		// Local transformation
		model = glm::translate(model, glm::vec3(t3d.localPosition.x, t3d.localPosition.y, t3d.localPosition.z));
		model = glm::rotate(model, glm::radians(t3d.localRotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(t3d.localRotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(t3d.localRotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(t3d.localScale.x, t3d.localScale.y, t3d.localScale.z));

		// Parent transformation
		glm::mat4 parent_model = glm::mat4(1.0f);
		parent_model = glm::translate(parent_model, glm::vec3(parent.position.x, parent.position.y, parent.position.z));
		parent_model = glm::rotate(parent_model, glm::radians(parent.rotation.x), glm::vec3(1, 0, 0));
		parent_model = glm::rotate(parent_model, glm::radians(parent.rotation.y), glm::vec3(0, 1, 0));
		parent_model = glm::rotate(parent_model, glm::radians(parent.rotation.z), glm::vec3(0, 0, 1));
		parent_model = glm::scale(parent_model, glm::vec3(parent.scale.x, parent.scale.y, parent.scale.z));

		model = parent_model * model;

		// Material bind
		Shader* matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(model, camera->getView(), camera->getProjection());
		SetUpLight(matShader, camera, directional, pointLights);

		material->Bind();

		mesh->Render();

		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}
	void Renderer3D::RenderMesh(Model* mesh, const Vector3f& position, const Vector3f& rotation, const Vector3f& scale, const size_t& directional,
		const std::vector<size_t>& pointLights, Material* material, bool clear, Camera* camera, bool cull)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
		material->getShader()->Bind();
		material->getShader()->SetMVP(model, camera->getView(), camera->getProjection());

		Shader* matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(model, camera->getView(), camera->getProjection());
		SetUpLight(matShader, camera, directional, pointLights);

		mesh->Render();

		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}
	void Renderer3D::RenderMesh(Model* mesh, const glm::mat4& transform, Material* material, const size_t& directional,
		const std::vector<size_t>& pointLights, bool clear, Camera* camera, bool cull)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		Shader* matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(transform, camera->getView(), camera->getProjection());
		SetUpLight(matShader, camera, directional, pointLights);

		material->Bind();

		mesh->Render();

		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, transform);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, transform);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}
	void Renderer3D::RenderSkybox()
	{
		{
			Camera* camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
			if (camera)
			{
				glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

				camera->frameBuffer->Bind(false);
				glDepthFunc(GL_LEQUAL);
				Shader* shader = m_DefaultSkybox.m_Material->getShader();
				shader->Bind();
				shader->setUniform(shader->getProjLoc(), camera->getProjection());
				shader->setUniform(shader->getViewLoc(), camera->getView());
				m_DefaultSkybox.Render();
				shader->UnBind();
				glDepthFunc(GL_LESS);

				camera->frameBuffer->Unbind();
			}
		}
		{
			Camera* camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
			if (camera)
			{
				glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

				camera->frameBuffer->Bind(false);
				glDepthFunc(GL_LEQUAL);
				Shader* shader = m_DefaultSkybox.m_Material->getShader();
				shader->Bind();
				shader->setUniform(shader->getProjLoc(), camera->getProjection());
				glm::mat4 view = glm::mat4(glm::mat3(camera->getView()));
				shader->setUniform(shader->getViewLoc(), view);
				m_DefaultSkybox.Render();
				shader->UnBind();
				glDepthFunc(GL_LESS);

				camera->frameBuffer->Unbind();
			}
		}
	}
	void Renderer3D::SetOption(Options option)
	{
		switch (option)
		{
		case Wiwa::Renderer3D::DEPTH_TEST:
			glEnable(GL_DEPTH_TEST);
			break;
		case Wiwa::Renderer3D::CULL_FACE:
			glEnable(GL_CULL_FACE);
			break;
		case Wiwa::Renderer3D::LIGHTING:
			glEnable(GL_LIGHTING);
			break;
		case Wiwa::Renderer3D::COLOR_MATERIAL:
			glEnable(GL_COLOR_MATERIAL);
			break;
		case Wiwa::Renderer3D::TEXTURE_2D:
			glEnable(GL_TEXTURE_2D);
			break;
		case Wiwa::Renderer3D::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		default:
			break;
		}
		
	}
	void Renderer3D::DisableOption(Options option)
	{
		switch (option)
		{
		case Wiwa::Renderer3D::DEPTH_TEST:
			glDisable(GL_DEPTH_TEST);
			break;
		case Wiwa::Renderer3D::CULL_FACE:
			glDisable(GL_CULL_FACE);
			break;
		case Wiwa::Renderer3D::LIGHTING:
			glDisable(GL_LIGHTING);
			break;
		case Wiwa::Renderer3D::COLOR_MATERIAL:
			glDisable(GL_COLOR_MATERIAL);
			break;
		case Wiwa::Renderer3D::TEXTURE_2D:
			glDisable(GL_TEXTURE_2D);
			break;
		case Wiwa::Renderer3D::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			break;
		}
	}
	void Renderer3D::Close()
	{
		
	}
	void Renderer3D::RenderFrustrums(Camera* camera)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		CameraManager& cameraManager = SceneManager::getActiveScene()->GetCameraManager();
		camera->frameBuffer->Bind(false);
		size_t cameraCount = cameraManager.getCameraSize();
		std::vector<CameraId>& cameras = cameraManager.getCameras();
		for (size_t i = 0; i < cameraCount; i++)
		{
			CameraId cam_id = cameras[i];
			Camera* cam = cameraManager.getCamera(cam_id);
			if (camera == cam)
				continue;

			m_BBDisplayShader->Bind();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cam->getPosition());
			model = glm::rotate(model, 0.0f, glm::vec3(1, 0, 0));
			model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));
			model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
			model = glm::scale(model, glm::vec3(1.0f));

			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			cam->DrawFrustrum();

			m_BBDisplayShader->UnBind();
		}
		camera->frameBuffer->Unbind();
	}
	void Renderer3D::SetUpLight(Wiwa::Shader* matShader, Wiwa::Camera* camera, const size_t& directional, const std::vector<size_t>& pointLights)
	{
		matShader->setUniform(matShader->getUniformLocation("u_CameraPosition"), camera->getPosition());
		if (directional != -1)
		{
			DirectionalLight* dirLight = SceneManager::getActiveScene()->GetEntityManager().GetComponent<DirectionalLight>(directional);
			if (dirLight)
			{
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Color"), dirLight->Color);
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.AmbientIntensity"), dirLight->AmbientIntensity);
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.DiffuseIntensity"), dirLight->DiffuseIntensity);
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Direction"), glm::radians(dirLight->Direction));
			}
		}

		
	}
}