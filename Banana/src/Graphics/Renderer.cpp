#include <Graphics/Renderer.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Banana
{
	Renderer* Renderer::m_Instance = nullptr;

	Renderer::Renderer()
		: m_ShaderColor("shaders/color/color.vert", "shaders/color/color.frag")
	{

	}

	void Renderer::SetProjection(float fov, float aspectRatio, float near, float far)
	{
		m_CurrentProjection = glm::perspective(fov, aspectRatio, near, far);
	}

	void Renderer::SetTarget(Ref<Framebuffer> target)
	{
		m_Target = target;
		if (!m_Target)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		else
			m_Target->Bind();
	}

	void Renderer::Clear(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginScene(Ref<Camera> cam, glm::vec3 pos)
	{
		m_CurrentView = cam->GetViewMatrix(pos);
	}

	void Renderer::EndScene()
	{
		m_CurrentView = glm::mat4(1.0f);
	}

	void Renderer::Draw(Ref<VAO> vao, Ref<EBO> ebo)
	{
		if (m_Target)
			m_Target->Bind();

		m_ShaderColor.Bind();

		vao->Bind();
		ebo->Bind();

		glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0);
	}

	Renderer* Renderer::Get()
	{
		if (!m_Instance)
			m_Instance = new Renderer;
		return m_Instance;
	}
}