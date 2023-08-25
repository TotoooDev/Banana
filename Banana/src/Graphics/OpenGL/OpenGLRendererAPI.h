#pragma once
#include <Graphics/RendererAPI.h>
#include <Graphics/Framebuffer.h>
#include <Events/Events.h>

namespace Banana
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();

		virtual void SetViewport(int x, int y, int width, int height) override;
		virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f) override;
		virtual void RenderScene() override;

	private:
		void ShadowPass();
		void ColorPass();

		std::vector<glm::vec4> GetFrustumCornersWorldSpace();
		void CalculateLightSpaceMatrix(glm::vec3 lightDir);

		void DrawObjects(Ref<Shader> shader);
		void DrawObject(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform);

		void OnWindowResized(WindowResizedEvent* event);

		#ifdef BANANA_OPENGL_DEBUG
			void InitOpenGLDebugOutput();
		#endif
	private:
		static bool m_WasGLEWInit;
		unsigned int m_ScreenWidth, m_ScreenHeight;

		Ref<Shader> m_ShaderDepth;
		Ref<Framebuffer> m_ShadowMap;
		glm::mat4 m_LightSpaceMatrix;
	};
}