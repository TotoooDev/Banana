#pragma once
#include <vector>

namespace Banana
{
	enum class FramebufferTexture
	{
		None,
		// Colors
		RGBA8,
		RGBA16,
		RGBA16F,
		// Depth
		Depth24Stencil8
	};

	class FramebufferSpecs
	{
	public:
		FramebufferSpecs() = default;

		void AddTexture(FramebufferTexture texture)
		{
			if (texture != FramebufferTexture::Depth24Stencil8)
				m_Textures.push_back(texture);
			else
				m_DepthTexture = texture;
		}

		unsigned int Width = 0;
		unsigned int Height = 0;
		bool Resizable = true;

	private:
		std::vector<FramebufferTexture> m_Textures;
		FramebufferTexture m_DepthTexture = FramebufferTexture::None;

		friend class Framebuffer;
	};

	class Framebuffer
	{
	public:
		Framebuffer();
		Framebuffer(const FramebufferSpecs& spec);

		void Bind();
		void Unbind();

		void SetSpecification(const FramebufferSpecs& spec) { m_Spec = spec; Recreate(); }
		void Recreate();
		void Resize(unsigned int width, unsigned int height);

		unsigned int RetrieveTexture(unsigned int id);
		void BindTexture(unsigned int id, unsigned int slot = 0);
		void BindAllTextures();

		void ClearAttachmentTexture(unsigned int id, unsigned int value);
		void ClearAllAttachmentTextures(unsigned int value);

		FramebufferSpecs GetSpec() const { return m_Spec; }
		unsigned int GetID() const { return m_ID; }

	private:
		void FramebufferTextureFormatToGL(FramebufferTexture texture, unsigned int* format, unsigned int* type);

		FramebufferSpecs m_Spec;

		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = -1;

		unsigned int m_ID = -1;
	};
}