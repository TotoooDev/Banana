#include <Graphics/Renderer.h>
#include <Core/Log.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Banana
{
	Renderer* Renderer::m_Instance = nullptr;

	Renderer::Renderer()
		: m_ShaderTexture("shaders/texture/texture.vert", "shaders/texture/texture.frag"),
		  m_ShaderColor("shaders/color/color.vert", "shaders/color/color.frag"),
		  m_ShaderCubemap("shaders/cubemap/cubemap.vert", "shaders/cubemap/cubemap.frag")
	{
		// Generate cubemap VAO
		std::vector<float> skyboxVertices = {
			// Positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			
			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			
			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		VertexLayout layout;
		layout.AddAttribute(Type::Vec3); // Positions
		m_CubemapVAO.SetData(skyboxVertices, layout);

		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		m_Projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	}

	void Renderer::OnViewportResize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void Renderer::ReloadShaders()
	{
		m_ShaderTexture.Reload();
		m_ShaderColor.Reload();
		m_ShaderCubemap.Reload();
	}

	void Renderer::SetCustomShader(Shader* shader)
	{
		m_CustomShader = shader;
		if (!m_CustomShader)
			return;

		m_CustomShader->Bind();
		m_CustomShader->SetMat4(m_CurrentView, "uView");
		m_CustomShader->SetMat4(m_Projection, "uProjection");
		m_CustomShader->SetVec3(m_CurrentCamPos, "uCamPos");
	}

	void Renderer::BeginScene(glm::vec3 camPos, glm::mat4 view)
	{
		m_CurrentCamPos = camPos;
		m_CurrentView = view;

		m_NumLights = 0;

		// Shader uniforms
		m_ShaderTexture.Bind();
		m_ShaderTexture.SetMat4(m_CurrentView, "uView");
		m_ShaderTexture.SetMat4(m_Projection, "uProjection");
		m_ShaderTexture.SetVec3(m_CurrentCamPos, "uCamPos");
		m_ShaderColor.Bind();
		m_ShaderColor.SetMat4(m_CurrentView, "uView");
		m_ShaderColor.SetMat4(m_Projection, "uProjection");
		m_ShaderTexture.SetVec3(m_CurrentCamPos, "uCamPos");
	}

	void Renderer::EndScene()
	{
		m_CurrentView = glm::mat4(1.0f);
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::AddDirectionalLight(const DirectionalLight& light)
	{
		AddDirectionalLight(light, m_ShaderTexture);
		AddDirectionalLight(light, m_ShaderColor);
	}

	void Renderer::AddPointLight(glm::vec3 pos, const PointLight& light)
	{
		AddPointLight(pos, light, m_ShaderTexture);
		AddPointLight(pos, light, m_ShaderColor);
	}

	void Renderer::AddSpotLight(glm::vec3 pos, const SpotLight& light)
	{
		AddSpotLight(pos, light, m_ShaderTexture);
		AddSpotLight(pos, light, m_ShaderColor);
	}

	void Renderer::Draw(const VAO& vao, const EBO& ebo, glm::mat4 transform, Material mat)
	{
		unsigned int diffuseSlot = 0;
		unsigned int specularSlot = 1;
		unsigned int normalSlot = 2;

		m_CurrentShader->Bind();
		m_CurrentShader->SetInt(diffuseSlot, "uMaterial.Diffuse");
		m_CurrentShader->SetInt(specularSlot, "uMaterial.Specular");
		m_CurrentShader->SetInt(normalSlot, "uMaterial.Normal");
		m_CurrentShader->SetFloat(mat.Shininess, "uMaterial.Shininess");

		if (mat.Diffuse)
			mat.Diffuse->Bind(diffuseSlot);
		if (mat.Specular)
			mat.Specular->Bind(specularSlot);
		if (mat.Normal)
			mat.Normal->Bind(normalSlot);

		vao.Bind();
		ebo.Bind();

		m_CurrentShader->SetMat4(transform, "uModel");

		glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, 0);

		m_CurrentShader = nullptr;
	}

	void Renderer::Draw(const VAO& vao, const EBO& ebo, glm::mat4 transform, glm::vec4 color)
	{
		m_CurrentShader->Bind();
		m_CurrentShader->SetVec4(color, "uColor");

		vao.Bind();
		ebo.Bind();

		m_CurrentShader->SetMat4(transform, "uModel");

		glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, 0);

		m_CurrentShader = nullptr;
	}

	void Renderer::Draw(const Mesh& mesh, glm::mat4 transform, Material mat)
	{
		Draw(*mesh.GetVAO().get(), *mesh.GetEBO().get(), transform * mesh.GetTransform(), mat);
	}

	void Renderer::Draw(const Mesh& mesh, glm::mat4 transform, glm::vec4 color)
	{
		Draw(*mesh.GetVAO().get(), *mesh.GetEBO().get(), transform * mesh.GetTransform(), color);
	}

	void Renderer::Draw(Ref<Model> model, glm::mat4 transform)
	{
		for (auto& mesh : model->GetMeshes())
		{
			if (model->GetMaterials().empty())
			{
				m_CurrentShader = m_CustomShader ? m_CustomShader : &m_ShaderColor;
				Draw(mesh, transform, glm::vec4(1.0f));
			}
			else
			{
				m_CurrentShader = m_CustomShader ? m_CustomShader : &m_ShaderTexture;
				Draw(mesh, transform, model->GetMaterials()[mesh.GetMaterialIndex()]);
			}
		}
	}

	void Renderer::Draw(Ref<Cubemap> cubemap)
	{
		m_CurrentShader = &m_ShaderCubemap;

		glDepthFunc(GL_LEQUAL);

		glm::mat4 view = glm::mat4(glm::mat3(m_CurrentView)); // Remove translation from view matrix

		m_ShaderCubemap.Bind();
		m_ShaderCubemap.SetMat4(view, "uView");
		m_ShaderCubemap.SetMat4(m_Projection, "uProjection");

		cubemap->Bind();
		m_CubemapVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_CubemapVAO.GetVertexCount());

		glDepthFunc(GL_LESS);

		m_CurrentShader = nullptr;
	}

	void Renderer::DrawInstance(const VAO& vao, const EBO& ebo, glm::mat4 transform, glm::vec4 color, int numInstances)
	{
		m_CurrentShader = m_CustomShader;

		m_CurrentShader->Bind();
		m_CurrentShader->SetVec4(color, "uColor");

		vao.Bind();
		ebo.Bind();

		m_CurrentShader->SetMat4(transform, "uModel");

		glDrawElementsInstanced(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, 0, numInstances);

		m_CurrentShader = nullptr;
	}

	Renderer* Renderer::Get()
	{
		if (!m_Instance)
			m_Instance = new Renderer;
		return m_Instance;
	}

	void Renderer::AddDirectionalLight(const DirectionalLight& light, Shader shader)
	{
		shader.Bind();
		shader.SetInt(0, "uLights[" + std::to_string(m_NumLights) + "].Type");
		shader.SetVec3(light.Direction, "uLights[" + std::to_string(m_NumLights) + "].Direction");
		shader.SetVec3(light.Ambient, "uLights[" + std::to_string(m_NumLights) + "].Ambient");
		shader.SetVec3(light.Diffuse, "uLights[" + std::to_string(m_NumLights) + "].Diffuse");
		shader.SetVec3(light.Specular, "uLights[" + std::to_string(m_NumLights) + "].Specular");

		m_NumLights++;
		shader.SetInt(m_NumLights, "uNumLights");
	}
	void Renderer::AddPointLight(glm::vec3 pos, const PointLight& light, Shader shader)
	{
		shader.Bind();
		shader.SetInt(1, "uLights[" + std::to_string(m_NumLights) + "].Type");
		shader.SetVec3(pos, "uLights[" + std::to_string(m_NumLights) + "].Pos");
		shader.SetVec3(light.Ambient, "uLights[" + std::to_string(m_NumLights) + "].Ambient");
		shader.SetVec3(light.Diffuse, "uLights[" + std::to_string(m_NumLights) + "].Diffuse");
		shader.SetVec3(light.Specular, "uLights[" + std::to_string(m_NumLights) + "].Specular");
		shader.SetFloat(light.Constant, "uLights[" + std::to_string(m_NumLights) + "].Constant");
		shader.SetFloat(light.Linear, "uLights[" + std::to_string(m_NumLights) + "].Linear");
		shader.SetFloat(light.Quadratic, "uLights[" + std::to_string(m_NumLights) + "].Quadratic");

		m_NumLights++;
		shader.SetInt(m_NumLights, "uNumLights");
	}
	void Renderer::AddSpotLight(glm::vec3 pos, const SpotLight& light, Shader shader)
	{
		shader.Bind();
		shader.SetInt(2, "uLights[" + std::to_string(m_NumLights) + "].Type");
		shader.SetVec3(light.Direction, "uLights[" + std::to_string(m_NumLights) + "].Direction");
		shader.SetVec3(light.Ambient, "uLights[" + std::to_string(m_NumLights) + "].Ambient");
		shader.SetVec3(light.Diffuse, "uLights[" + std::to_string(m_NumLights) + "].Diffuse");
		shader.SetVec3(light.Specular, "uLights[" + std::to_string(m_NumLights) + "].Specular");
		shader.SetFloat(light.Constant, "uLights[" + std::to_string(m_NumLights) + "].Constant");
		shader.SetFloat(light.Linear, "uLights[" + std::to_string(m_NumLights) + "].Linear");
		shader.SetFloat(light.Quadratic, "uLights[" + std::to_string(m_NumLights) + "].Quadratic");
		shader.SetFloat(light.CutOff, "uLights[" + std::to_string(m_NumLights) + "].CutOff");
		shader.SetFloat(light.OuterCutOff, "uLights[" + std::to_string(m_NumLights) + "].OuterCutOff");

		m_NumLights++;
		shader.SetInt(m_NumLights, "uNumLights");
	}
}