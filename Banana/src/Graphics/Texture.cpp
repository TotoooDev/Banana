#include <Core/Log.h>
#include <Graphics/Texture.h>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Banana
{
	Texture::Texture(bool flipUVs)
	{
		stbi_set_flip_vertically_on_load(flipUVs);
		glGenTextures(1, &m_ID);
	}

	Texture::Texture(const std::string& path, bool flipUVs)
	{
		stbi_set_flip_vertically_on_load(flipUVs);
		glGenTextures(1, &m_ID);
		CreateFromFile(path);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Texture::CreateFromFile(const std::string& path)
	{
		BANANA_INFO("Loading texture {}", path);

		glBindTexture(GL_TEXTURE_2D, m_ID);

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, numChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
		if (data == nullptr)
		{
			BANANA_ERROR("Failed to load image {}!", path);
			return;
		}

		GLenum glFormat = GL_RGB;
		if (numChannels == 3)
		{
			glFormat = GL_RGB;
		}
		if (numChannels == 4)
		{
			glFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	void Texture::Bind(unsigned int slot) const
	{
		// Activate the texture unit first before binding texture
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	unsigned int Texture::GetID() const
	{
		return m_ID;
	}
}