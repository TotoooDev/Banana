#include <Core/Log.h>
#include <Graphics/Cubemap.h>
#include <GL/glew.h>
#include <stb_image.h>

namespace Banana
{
	Cubemap::Cubemap()
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}

	Cubemap::Cubemap(std::vector<std::string> facePaths)
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		CreateFromFiles(facePaths);
	}

	Cubemap::~Cubemap()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Cubemap::Bind(unsigned int slot)
	{
		// Activate the texture unit first before binding texture
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}

	void Cubemap::CreateFromFiles(std::vector<std::string> facePaths)
	{
		int width, height, nbChannels;
		for (unsigned int i = 0; i < facePaths.size(); i++)
		{
			BANANA_INFO("Loading texture {0} for cubemap {1}", facePaths[i], m_ID);
			unsigned char* data = stbi_load(facePaths[i].c_str(), &width, &height, &nbChannels, 0);
			if (!data)
			{
				BANANA_ERROR("Failed to load image {}!", facePaths[i]);
				stbi_image_free(data);
				continue;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
	}
}