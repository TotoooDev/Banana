#include <Core/Log.h>
#include <Graphics/OpenGL/OpenGLTexture.h>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Banana
{
	OpenGLTexture::OpenGLTexture(TextureSpecs specs)
	{
		m_Specs = specs;
		CreateTexture();
	}

	OpenGLTexture::OpenGLTexture(TextureSpecs specs, const std::string& path, bool flip)
	{
		m_Specs = specs;
		CreateTexture();
		LoadFromFile(path, flip);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture::LoadFromFile(const std::string& path, bool flip)
	{
		if (path.empty())
		{
			BANANA_ERROR("Texture path is empty!");
			return;
		}

		stbi_set_flip_vertically_on_load(flip);

		BANANA_INFO("Loading image {}", path);
		int width, height, numChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
		if (!data)
		{
			BANANA_ERROR("Failed to load image {}!", path);
			stbi_image_free(data);
			return;
		}

		unsigned int openglChannels = 0;
		switch (numChannels)
		{
		case 1: openglChannels = GL_R; break;
		case 2: openglChannels = GL_RG; break;
		case 3: openglChannels = GL_RGB; break;
		case 4: openglChannels = GL_RGBA; break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, openglChannels, width, height, 0, openglChannels, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	void OpenGLTexture::Bind(unsigned int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void OpenGLTexture::CreateTexture()
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		// Wrapping mode
		switch (m_Specs.WrappingMode)
		{
		case TextureWrapping::Repeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TextureWrapping::MirroredRepeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		case TextureWrapping::ClampToEdge:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case TextureWrapping::ClampToBorder:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			// Set the border color
			float color[] = { 1.0f, 0.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
			break;
		}

		// Filtering mode
		switch (m_Specs.FilteringMode)
		{
		case TextureFiltering::Nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case TextureFiltering::Linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}

		// Generate the texture if the size isn't negative
		if (m_Specs.Width > 0 || m_Specs.Height > 0)
		{
			unsigned int numChannels = 0;
			switch (m_Specs.Channels)
			{
			case TextureChannels::R: numChannels = GL_R; break;
			case TextureChannels::RG: numChannels = GL_RG; break;
			case TextureChannels::RGB: numChannels = GL_RGB; break;
			case TextureChannels::RGBA: numChannels = GL_RGBA; break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, numChannels, m_Specs.Width, m_Specs.Height, 0, numChannels, GL_UNSIGNED_BYTE, nullptr);
		}
	}
}