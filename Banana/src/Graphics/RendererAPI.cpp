#include <Core/Log.h>
#include <Graphics/RendererAPI.h>
#include <Graphics/OpenGL/OpenGLRendererAPI.h>
#include <Core/Application.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Banana
{
	RendererAPIName RendererAPI::m_API = RendererAPIName::None;

	Ref<RendererAPI> RendererAPI::Create(RendererAPIName api)
	{
		m_API = api;
		switch (api)
		{
		case RendererAPIName::OpenGL:
			return CreateRef<OpenGLRendererAPI>();
		case RendererAPIName::Vulkan:
			BANANA_ABORT("Vulkan is not supported yet :(");
		case RendererAPIName::None:
		default:
			BANANA_ABORT("Unsupported renderer API {}!", (int)api);
		}

		return nullptr;
	}

	void RendererAPI::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		m_CurrentProjection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	}

	void RendererAPI::SetTarget(Ref<Framebuffer> target)
	{
		m_Target = target;
	}

	void RendererAPI::BeginScene(Ref<Camera> cam, glm::vec3 camPos)
	{
		m_CurrentView = cam->GetViewMatrix(camPos);
		
		// Clear the draw list
		m_ObjectsToDraw.clear();
		
		// Clear the light lists
		m_NumLights = 0;
		m_DirectionalLights.clear();
		m_PointLights.clear();
		m_SpotLights.clear();
	}

	void RendererAPI::Draw(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform)
	{
		m_ObjectsToDraw.push_back({ vao, ebo, material, transform });
	}
	void RendererAPI::Draw(Ref<Mesh> mesh, Material material, glm::mat4 transform)
	{
		Draw(mesh->GetVAO(), mesh->GetEBO(), material, transform);
	}
	void RendererAPI::Draw(Ref<Model> model, std::vector<Material> materials, glm::mat4 transform)
	{
		for (auto& mesh : model->GetMeshes())
			Draw(mesh, materials[mesh->GetMaterialIndex()], transform);
	}

	void RendererAPI::AddDirectionalLight(const DirectionalLight& light, glm::vec3 direction)
	{
		m_DirectionalLights.push_back(std::make_pair(light, direction));
		AddDirectionalLight(m_ShaderColor, light, direction);
		AddDirectionalLight(m_ShaderTexture, light, direction);
		if (m_CustomShader)
			AddDirectionalLight(m_CustomShader, light, direction);
	}
	void RendererAPI::AddPointLight(const PointLight& light, glm::vec3 pos)
	{
		m_PointLights.push_back(light);
		AddPointLight(m_ShaderColor, light, pos);
		AddPointLight(m_ShaderTexture, light, pos);
		if (m_CustomShader)
			AddPointLight(m_CustomShader, light, pos);
	}
	void RendererAPI::AddSpotLight(const SpotLight& light, glm::vec3 pos, glm::vec3 direction)
	{
		m_SpotLights.push_back(light);
		AddSpotLight(m_ShaderColor, light, pos, direction);
		AddSpotLight(m_ShaderTexture, light, pos, direction);
		if (m_CustomShader)
			AddSpotLight(m_CustomShader, light, pos, direction);
	}

	void RendererAPI::AddDirectionalLight(Ref<Shader> shader, const DirectionalLight& light, glm::vec3 direction)
	{
		shader->Bind();
		shader->SetInt(0, "uLights[" + std::to_string(m_NumLights) + "].Type");
		shader->SetVec3(direction, "uLights[" + std::to_string(m_NumLights) + "].Direction");
		shader->SetVec3(light.Ambient, "uLights[" + std::to_string(m_NumLights) + "].Ambient");
		shader->SetVec3(light.Diffuse, "uLights[" + std::to_string(m_NumLights) + "].Diffuse");
		shader->SetVec3(light.Specular, "uLights[" + std::to_string(m_NumLights) + "].Specular");

		m_NumLights++;
		shader->SetInt(m_NumLights, "uNumLights");
	}
	void RendererAPI::AddPointLight(Ref<Shader> shader, const PointLight& light, glm::vec3 pos)
	{
		shader->Bind();
		shader->SetInt(1, "uLights[" + std::to_string(m_NumLights) + "].Type");
		shader->SetVec3(pos, "uLights[" + std::to_string(m_NumLights) + "].Pos");
		shader->SetVec3(light.Ambient, "uLights[" + std::to_string(m_NumLights) + "].Ambient");
		shader->SetVec3(light.Diffuse, "uLights[" + std::to_string(m_NumLights) + "].Diffuse");
		shader->SetVec3(light.Specular, "uLights[" + std::to_string(m_NumLights) + "].Specular");
		shader->SetFloat(light.Constant, "uLights[" + std::to_string(m_NumLights) + "].Constant");
		shader->SetFloat(light.Linear, "uLights[" + std::to_string(m_NumLights) + "].Linear");
		shader->SetFloat(light.Quadratic, "uLights[" + std::to_string(m_NumLights) + "].Quadratic");

		m_NumLights++;
		shader->SetInt(m_NumLights, "uNumLights");
	}
	void RendererAPI::AddSpotLight(Ref<Shader> shader, const SpotLight& light, glm::vec3 pos, glm::vec3 direction)
	{
		shader->Bind();
		shader->SetInt(2, "uLights[" + std::to_string(m_NumLights) + "].Type");
		shader->SetVec3(pos, "uLights[" + std::to_string(m_NumLights) + "].Pos");
		shader->SetVec3(direction, "uLights[" + std::to_string(m_NumLights) + "].Direction");
		shader->SetVec3(light.Ambient, "uLights[" + std::to_string(m_NumLights) + "].Ambient");
		shader->SetVec3(light.Diffuse, "uLights[" + std::to_string(m_NumLights) + "].Diffuse");
		shader->SetVec3(light.Specular, "uLights[" + std::to_string(m_NumLights) + "].Specular");
		shader->SetFloat(light.Constant, "uLights[" + std::to_string(m_NumLights) + "].Constant");
		shader->SetFloat(light.Linear, "uLights[" + std::to_string(m_NumLights) + "].Linear");
		shader->SetFloat(light.Quadratic, "uLights[" + std::to_string(m_NumLights) + "].Quadratic");
		shader->SetFloat(light.CutOff, "uLights[" + std::to_string(m_NumLights) + "].CutOff");
		shader->SetFloat(light.OuterCutOff, "uLights[" + std::to_string(m_NumLights) + "].OuterCutOff");

		m_NumLights++;
		shader->SetInt(m_NumLights, "uNumLights");
	}
}