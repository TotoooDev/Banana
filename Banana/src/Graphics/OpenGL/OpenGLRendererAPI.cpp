#include <Core/Log.h>
#include <Graphics/OpenGL/OpenGLRendererAPI.h>
#include <Core/Application.h>
#include <glm/gtc/matrix_transform.hpp>

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
		m_ShaderDepth = Shader::Create("shaders/depth/depth.vert", "shaders/depth/depth.frag");

		// Create framebuffers
		FramebufferSpecs specs;
		specs.Width = 4096;
		specs.Height = 4096;
		specs.AddTexture(FramebufferTexture::Depth24Stencil8);
		m_ShadowMap = Framebuffer::Create(specs);

		// Set other stuff
		m_ScreenWidth = Application::Get()->GetWindowSpecs().Width;
		m_ScreenHeight = Application::Get()->GetWindowSpecs().Height;

		Application::Get()->GetEventBus()->Subscribe(this, &OpenGLRendererAPI::OnWindowResized);
	}

	void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::RenderScene()
	{
		ShadowPass();
		ColorPass();
	}

	void OpenGLRendererAPI::ShadowPass()
	{
		SetViewport(0, 0, m_ShadowMap->GetSpec().Width, m_ShadowMap->GetSpec().Height);
		CalculateLightSpaceMatrix(m_DirectionalLights[0].second);
		m_ShadowMap->Bind();
		{
			Clear();
			glCullFace(GL_FRONT);
			DrawObjects(m_ShaderDepth);
			glCullFace(GL_BACK);
		}
		m_ShadowMap->Unbind();
	}

	void OpenGLRendererAPI::ColorPass()
	{
		SetViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
		Clear();
		m_ShadowMap->BindTexture(0); // Bind the depth texture (it's the only texture so we know it's index 0)
		DrawObjects(m_ShaderColor);
	}

	std::vector<glm::vec4> OpenGLRendererAPI::GetFrustumCornersWorldSpace()
	{
		glm::mat4 inverse = glm::inverse(m_CurrentProjection * m_CurrentView);

		std::vector<glm::vec4> frustumCorners;
		for (unsigned int x = 0; x < 2; x++)
		{
			for (unsigned int y = 0; y < 2; y++)
			{
				for (unsigned int z = 0; z < 2; z++)
				{
					glm::vec4 pt = inverse * glm::vec4(
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						2.0f * z - 1.0f,
						1.0f
					);
					frustumCorners.push_back(pt / pt.w);
				}
			}
		}

		return frustumCorners;
	}

	void OpenGLRendererAPI::CalculateLightSpaceMatrix(glm::vec3 lightDir)
	{
		// Calculate the light view matrix
		std::vector<glm::vec4> corners = GetFrustumCornersWorldSpace();
		glm::vec3 center = glm::vec3(0.0f);
		for (auto& v : corners)
			center += glm::vec3(v);
		center /= corners.size();
		glm::mat4 lightView = glm::lookAt(center - lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

		// Calculate the light projection matrix
		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::lowest();
		for (auto& v : corners)
		{
			glm::vec4 trf = lightView * v;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
		
		m_LightSpaceMatrix = lightProjection * lightView;
	}

	void OpenGLRendererAPI::DrawObjects(Ref<Shader> shader)
	{
		m_CurrentShader = m_CustomShader ? m_CustomShader : shader;
		for (auto& object : m_ObjectsToDraw)
			DrawObject(object.VAO, object.EBO, object.Mat, object.Transform);
		m_CurrentShader = nullptr;
	}

	void OpenGLRendererAPI::DrawObject(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform)
	{
		if (m_Target)
			m_Target->Bind();

		if (material.DrawWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_CurrentShader->Bind();
		m_CurrentShader->SetMat4(transform, "uModel");
		m_CurrentShader->SetMat4(m_CurrentView, "uView");
		m_CurrentShader->SetMat4(m_CurrentProjection, "uProjection");

		m_CurrentShader->SetInt(material.UseColors, "uMaterial.UseColors");
		m_CurrentShader->SetVec3(material.ColorDiffuse, "uMaterial.ColorDiffuse");
		m_CurrentShader->SetVec3(material.ColorSpecular, "uMaterial.ColorSpecular");
		if (material.TextureDiffuse)
			m_CurrentShader->SetTexture(material.TextureDiffuse, "uMaterial.TextureDiffuse");
		if (material.TextureSpecular)
			m_CurrentShader->SetTexture(material.TextureSpecular, "uMaterial.TextureSpecular");

		m_CurrentShader->SetMat4(m_LightSpaceMatrix, "uLightSpaceMatrix");

		vao->Bind();
		ebo->Bind();

		glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0);
	}

	void OpenGLRendererAPI::OnWindowResized(WindowResizedEvent* event)
	{
		if (event->Width == 0 || event->Height == 0)
			return;

		m_ScreenWidth = event->Width;
		m_ScreenHeight = event->Height;
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