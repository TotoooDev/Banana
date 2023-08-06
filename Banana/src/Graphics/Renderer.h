#pragma once
#include <Graphics/Camera.h>
#include <Graphics/VertexObjects.h>
#include <Graphics/Mesh.h>
#include <Graphics/Cubemap.h>
#include <Graphics/Shader.h>
#include <Graphics/Light.h>
#include <vector>

namespace Banana
{
	class Renderer
	{
	public:
		Renderer(Renderer&) = delete;
		void operator=(const Renderer&) = delete;

		static Renderer* Get();

		void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
		void OnViewportResize(int width, int height);

		void ReloadShaders();
		void SetCustomShader(Shader* shader);

		void BeginScene(glm::vec3 camPos, glm::mat4 view);
		void EndScene();

		void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0);

		void AddDirectionalLight(const DirectionalLight& light);
		void AddPointLight(glm::vec3 pos, const PointLight& light);
		void AddSpotLight(glm::vec3 pos, const SpotLight& light);

		void Draw(const VAO& vao, const EBO& ebo, glm::mat4 transform, Material mat);
		void Draw(const VAO& vao, const EBO& ebo, glm::mat4 transform, glm::vec4 color);
		void Draw(const Mesh& mesh, glm::mat4 transform, Material mat);
		void Draw(const Mesh& mesh, glm::mat4 transform, glm::vec4 color);
		void Draw(Ref<Model> model, glm::mat4 transform);
		void Draw(Ref<Cubemap> cubemap);

		// Instances
		void DrawInstance(const VAO& vao, const EBO& ebo, glm::mat4 transform, glm::vec4 color, int numInstances);

	private:
		Renderer();
		void AddDirectionalLight(const DirectionalLight& light, Shader shader);
		void AddPointLight(glm::vec3 pos, const PointLight& light, Shader shader);
		void AddSpotLight(glm::vec3 pos, const SpotLight& light, Shader shader);

	private:
		static Renderer* m_Instance;

		glm::vec3 m_CurrentCamPos;
		glm::mat4 m_CurrentView;
		glm::mat4 m_Projection;

		Shader m_ShaderTexture;
		Shader m_ShaderColor;
		Shader m_ShaderCubemap;
		Shader* m_CustomShader = nullptr;
		Shader* m_CurrentShader = nullptr;

		VAO m_CubemapVAO;

		unsigned int m_NumLights = 0;
	};
}