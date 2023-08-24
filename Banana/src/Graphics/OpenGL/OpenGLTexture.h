#pragma once
#include <Graphics/Texture.h>

namespace Banana
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(TextureSpecs specs);
		OpenGLTexture(TextureSpecs specs, const std::string& path, bool flip = false);
		~OpenGLTexture();

		virtual void LoadFromFile(const std::string& path, bool flip = false) override;
		virtual void Bind(unsigned int unit = 0) override;

		virtual unsigned int GetID() override { return m_ID; }

	private:
		void CreateTexture();

	private:
		unsigned int m_ID;
	};
}