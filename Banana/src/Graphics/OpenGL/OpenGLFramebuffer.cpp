#include <Graphics/Log.h>
#include <Graphics/OpenGL/OpenGLFramebuffer.h>
#include <GL/glew.h>

namespace Banana
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs)
	{
		m_Specs = specs;
		Recreate();
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Recreate()
	{
		if (m_ID != -1)
			glDeleteFramebuffers(1, &m_ID);

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		m_ColorAttachments.clear();

		std::vector<GLenum> attachments;
		for (unsigned int i = 0; i < m_Specs.GetTextures().size(); i++)
		{
			GLenum format, type;

			FramebufferTextureFormatToGL(m_Specs.GetTextures()[i], &format, &type);

			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Specs.Width, m_Specs.Height, 0, GL_RGBA, type, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureID, 0);
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

			m_ColorAttachments.push_back(textureID);
		}

		if (m_Specs.GetDepthTexture() != FramebufferTexture::None)
		{
			unsigned int id;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Specs.Width, m_Specs.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
			m_DepthAttachment = id;
			attachments.push_back(GL_COLOR_ATTACHMENT0 + attachments.size());

			m_ColorAttachments.push_back(id);
		}

		glDrawBuffers(attachments.size(), attachments.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			BANANA_RENDERER_ERROR("Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(unsigned int width, unsigned int height)
	{
		if (!m_Specs.Resizable)
		{
			BANANA_RENDERER_WARN("Attempting to resize unresizable framebuffer!");
			return;
		}
		m_Specs.Width = width;
		m_Specs.Height = height;
		glViewport(0, 0, width, height);
		Recreate();
	}

	void OpenGLFramebuffer::BindTexture(unsigned int id, unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[id]);
	}

	void OpenGLFramebuffer::BindAllTextures()
	{
		for (unsigned int i = 0; i < m_ColorAttachments.size(); i++)
		{
			BindTexture(i, i);
		}
	}

	void OpenGLFramebuffer::ClearAttachmentTexture(unsigned int id, unsigned int value)
	{
		unsigned int format, type;
		FramebufferTextureFormatToGL(m_Specs.GetTextures()[id], &format, &type);
		glClearTexImage(m_ColorAttachments[id], 0, GL_RGBA, type, &value);
	}

	void OpenGLFramebuffer::ClearAllAttachmentTextures(unsigned int value)
	{
		Bind();
		glClearColor(value >> 24, value >> 16, value >> 8, value);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Unbind();
	}

	void OpenGLFramebuffer::FramebufferTextureFormatToGL(FramebufferTexture texture, unsigned int* format, unsigned int* type)
	{
		switch (texture)
		{
		default:								  *format = GL_RGBA8;            *type = GL_UNSIGNED_BYTE; break;
		case FramebufferTexture::RGBA8:           *format = GL_RGBA8;            *type = GL_UNSIGNED_BYTE; break;
		case FramebufferTexture::RGBA16:          *format = GL_RGBA16;           *type = GL_FLOAT;         break;
		case FramebufferTexture::RGBA16F:         *format = GL_RGBA16F;          *type = GL_FLOAT;         break;
		case FramebufferTexture::Depth24Stencil8: *format = GL_DEPTH24_STENCIL8; *type = GL_FLOAT;         break;
		}
	}
}