#include <Core/Log.h>
#include <Graphics/Texture.h>
#include <GL/glew.h>
#include <stb_image.h>

namespace Banana
{
	Texture::Texture(TextureSpecs specs)
		: m_Specs(specs)
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
		if (m_Specs.Width < 0 || m_Specs.Height < 0)
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

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Texture::LoadFromFile(const std::string& path, bool flip)
	{
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

	void Texture::Bind(unsigned int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}