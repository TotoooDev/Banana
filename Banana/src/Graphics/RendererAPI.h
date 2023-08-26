#pragma once
#include <Core/Ref.h>
#include <Graphics/Shader.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/Model.h>
#include <Graphics/Camera.h>
#include <Graphics/Light.h>
#include <glm/glm.hpp>

namespace Banana
{
	enum class RendererAPIName
	{
		None,
		OpenGL,
		Vulkan
	};

	class RendererAPI
	{
	public:
		static Ref<RendererAPI> Create(RendererAPIName api);
		static RendererAPIName GetAPI() { return m_API; }

		void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
		void SetTarget(Ref<Framebuffer> target);
		void BeginScene(Ref<Camera> cam, glm::vec3 camPos);

		void AddDirectionalLight(const DirectionalLight& light, glm::vec3 direction);
		void AddPointLight(const PointLight& light, glm::vec3 pos);
		void AddSpotLight(const SpotLight& light, glm::vec3 pos, glm::vec3 direction);

		void Draw(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform);
		void Draw(Ref<Mesh> mesh, Material material, glm::mat4 transform);
		void Draw(Ref<Model> model, std::vector<Material> materials, glm::mat4 transform);

		// Platform specific methods
		virtual void SetViewport(int x, int y, int width, int height) = 0;
		virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;
		virtual void RenderScene() = 0;

	protected:
		glm::mat4 m_CurrentView = glm::mat4(1.0f);
		glm::mat4 m_CurrentProjection = glm::mat4(1.0f);

		Ref<Shader> m_CurrentShader = nullptr;
		Ref<Shader> m_CustomShader = nullptr;
		Ref<Shader> m_ShaderColor;
		Ref<Shader> m_ShaderTexture;

		Ref<Framebuffer> m_Target = nullptr;

		struct DrawableObject
		{
			Ref<Banana::VAO> VAO;
			Ref<Banana::EBO> EBO;
			Material Mat;
			glm::mat4 Transform;
		};
		std::vector<DrawableObject> m_ObjectsToDraw;
		std::vector<std::pair<DirectionalLight, glm::vec3>> m_DirectionalLights;
		std::vector<PointLight> m_PointLights;
		std::vector<SpotLight> m_SpotLights;

	private:
		void AddDirectionalLight(Ref<Shader> shader, const DirectionalLight& light, glm::vec3 direction);
		void AddPointLight(Ref<Shader> shader, const PointLight& light, glm::vec3 pos);
		void AddSpotLight(Ref<Shader> shader, const SpotLight& light, glm::vec3 pos, glm::vec3 direction);

	private:
		static RendererAPIName m_API;

		unsigned int m_NumLights;
	};
}