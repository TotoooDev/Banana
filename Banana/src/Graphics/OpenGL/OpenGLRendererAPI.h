#pragma once
#include <Graphics/RendererAPI.h>

namespace Banana
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();

		virtual void SetViewport(int x, int y, int width, int height) override;
		virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f) override;

		virtual void Draw(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform) override;
		virtual void Draw(Ref<Mesh> mesh, Material material, glm::mat4 transform) override;
		virtual void Draw(Ref<Model> model, std::vector<Material> materials, glm::mat4 transform) override;

	private:
		void DrawTextured(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform);
		void DrawColor(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform);

		#ifdef BANANA_OPENGL_DEBUG
			void InitOpenGLDebugOutput();
		#endif
	private:
		static bool m_WasGLEWInit;
	};
}