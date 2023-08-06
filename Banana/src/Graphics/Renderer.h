#pragma once
#include <Core/Ref.h>
#include <Graphics/Shader.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/VertexObjects.h>
#include <Graphics/Camera.h>

namespace Banana
{
	class Renderer
	{
	public:
		Renderer(Renderer&) = delete;
		void operator=(const Renderer&) = delete;

		static Renderer* Get();

		void SetProjection(float fov, float aspectRatio, float near, float far);
		void OnViewportResize(unsigned int width, unsigned int height) {}

		// Set to nullptr to draw to the screen
		void SetTarget(Ref<Framebuffer> target);

		void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f);
		void BeginScene(Ref<Camera> cam, glm::vec3 pos);
		void EndScene();

		void Draw(Ref<VAO> vao, Ref<EBO> ebo);

	private:
		Renderer();

	private:
		static Renderer* m_Instance;

		glm::mat4 m_CurrentView = glm::mat4(1.0f);
		glm::mat4 m_CurrentProjection = glm::mat4(1.0f);

		Ref<Framebuffer> m_Target = nullptr;

		Shader m_ShaderColor;
	};
}