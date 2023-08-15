#pragma once
#include <Graphics/Framebuffer.h>

namespace Banana
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecs& specs);

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetSpecification(const FramebufferSpecs& spec) { m_Specs = spec; Recreate(); }
		virtual void Recreate() override;
		virtual void Resize(unsigned int width, unsigned int height) override;

		virtual void BindTexture(unsigned int id, unsigned int slot = 0) override;
		virtual void BindAllTextures() override;

		virtual void ClearAttachmentTexture(unsigned int id, unsigned int value) override;
		virtual void ClearAllAttachmentTextures(unsigned int value) override;

	private:
		void FramebufferTextureFormatToGL(FramebufferTexture texture, unsigned int* format, unsigned int* type);

	private:
		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = -1;

		unsigned int m_ID = -1;
	};
}