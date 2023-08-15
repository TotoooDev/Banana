#pragma once
#include <Core/Ref.h>
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

		std::vector<FramebufferTexture> GetTextures() const { return m_Textures; }
		FramebufferTexture GetDepthTexture() const { return m_DepthTexture; }

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
		static Ref<Framebuffer> Create(const FramebufferSpecs& specs);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetSpecification(const FramebufferSpecs& spec) = 0;
		virtual void Recreate() = 0;
		virtual void Resize(unsigned int width, unsigned int height) = 0;

		virtual void BindTexture(unsigned int id, unsigned int slot = 0) = 0;
		virtual void BindAllTextures() = 0;

		virtual void ClearAttachmentTexture(unsigned int id, unsigned int value) = 0;
		virtual void ClearAllAttachmentTextures(unsigned int value) = 0;

		FramebufferSpecs GetSpec() const { return m_Specs; }

	protected:
		FramebufferSpecs m_Specs;

	private:
		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = -1;

		unsigned int m_ID = -1;
	};
}