#include <Core/Log.h>
#include <Graphics/OpenGL/OpenGLRendererAPI.h>

namespace Banana
{
	bool OpenGLRendererAPI::m_WasGLEWInit = false;

	OpenGLRendererAPI::OpenGLRendererAPI()
	{
		// Initialize glew
		if (!m_WasGLEWInit)
		{
			glewInit();
			m_WasGLEWInit = true;
		}

		#ifdef BANANA_OPENGL_DEBUG
			// OpenGL debug context
			InitOpenGLDebugOutput();
		#endif

		glEnable(GL_DEPTH_TEST);

		// Compile shaders
		m_ShaderColor = Shader::Create("shaders/color/color.vert", "shaders/color/color.frag");
		m_ShaderTexture = Shader::Create("shaders/texture/texture.vert", "shaders/texture/texture.frag");
	}

	void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Draw(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform)
	{
		// Use the custom shader if it points to something
		if (material.UseColors)
		{
			m_CurrentShader = m_CustomShader ? m_CustomShader : m_ShaderColor;
			DrawColor(vao, ebo, material, transform);
		}
		else
		{
			m_CurrentShader = m_CustomShader ? m_CustomShader : m_ShaderTexture;
			DrawTextured(vao, ebo, material, transform);
		}

		m_CurrentShader = nullptr;
	}

	void OpenGLRendererAPI::Draw(Mesh mesh, Material material, glm::mat4 transform)
	{
		Draw(mesh.GetVAO(), mesh.GetEBO(), material, transform);
	}

	void OpenGLRendererAPI::Draw(Ref<Model> model, std::vector<Material> materials, glm::mat4 transform)
	{
		for (auto& mesh : model->GetMeshes())
			Draw(mesh, materials[mesh.GetMaterialIndex()], transform);
	}

	void OpenGLRendererAPI::DrawTextured(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform)
	{

	}

	void OpenGLRendererAPI::DrawColor(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform)
	{
		if (m_Target)
			m_Target->Bind();

		m_CurrentShader->Bind();
		m_CurrentShader->SetMat4(transform, "uModel");
		m_CurrentShader->SetMat4(m_CurrentView, "uView");
		m_CurrentShader->SetMat4(m_CurrentProjection, "uProjection");
		m_CurrentShader->SetVec3(material.ColorDiffuse, "uColor");

		vao->Bind();
		ebo->Bind();

		glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0);
	}

	#ifdef BANANA_DEBUG
	void OpenGLRendererAPI::InitOpenGLDebugOutput()
	{
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			// Set the callback as a lambda
			// Shamelessly stolen from https://learnopengl.com/In-Practice/Debugging
			glDebugMessageCallback(
				[](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
				{
					// Ignore non-significant error/warning codes
					if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
						return;

					std::stringstream sstream;
					sstream << message << " ";

					switch (severity)
					{
					case GL_DEBUG_SEVERITY_HIGH:         BANANA_ERROR(sstream.str()); break;
					case GL_DEBUG_SEVERITY_MEDIUM:       BANANA_WARN(sstream.str()); break;
					case GL_DEBUG_SEVERITY_LOW:          BANANA_WARN(sstream.str()); break;
					case GL_DEBUG_SEVERITY_NOTIFICATION: BANANA_INFO(sstream.str()); break;
					}
				}, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		else
		{
			BANANA_ERROR("Unable to create a debug context!");
		}
	}
	#endif
}