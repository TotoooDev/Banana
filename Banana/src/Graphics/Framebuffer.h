#pragma once
#include <Core/Ref.h>
#include <vector>

namespace Banana
{
	/**
	 * An enum to represent different types of frambuffer textures you can use.
	 */
	enum class FramebufferTexture
	{
		None, /** No texture */
		RGBA8, /** An 8-bit per channel RGBA color. */
		RGBA16, /** A 16-bit per channel RGBA color. */
		RGBA16F, /** A 16-bit per channel RGBA color (floating point). */

		Depth24Stencil8 /** A 24-bit depth buffer + an 8-bit stencil buffer. */
	};

	/**
	 * A framebuffer specification.
	 */
	class FramebufferSpecs
	{
	public:
		FramebufferSpecs() = default;

		/**
		 * Adds a texture to the specification.
		 * Adding one of the depth framebuffer textures will result in the m_DepthTexture member set to this texture and used as the depth texture for the frambuffer.
		 * @param texture - The framebuffer texture type to add
		 */
		void AddTexture(FramebufferTexture texture);

	// Getters
	public:
		/**
		 * @returns The framebuffer textures used in the specification.
		 */
		std::vector<FramebufferTexture> GetTextures() const { return m_Textures; }
		/**
		 * @returns The depth texture used in the specification.
		 */
		FramebufferTexture GetDepthTexture() const { return m_DepthTexture; }

		unsigned int Width = 0; /** The width of the framebuffer. */
		unsigned int Height = 0; /** The height of the framebuffer. */
		bool Resizable = true; /** Specifies whether the framebuffer is resizable or not. */

	private:
		std::vector<FramebufferTexture> m_Textures;
		FramebufferTexture m_DepthTexture = FramebufferTexture::None;

		friend class Framebuffer;
	};

	/**
	 * Provides an API for the framebuffer object. Its implementation depends on the graphics specification.
	 */
	class Framebuffer
	{
	public:
		/**
		 * @returns A reference to the newly created framebuffer.
		 * @param specs - The specification to use for this frmabuffer.
		 */
		static Ref<Framebuffer> Create(const FramebufferSpecs& specs);

		/**
		 * Binds the framebuffer for future uses. After calling this method, all futures renders will be done to this framebuffer.
		 */
		virtual void Bind() = 0;

		/**
		 * Unbinds the framebuffer. After calling this method, all futures renders will be done to the screen.
		 */
		virtual void Unbind() = 0;

		/**
		 * Recreates the framebuffer with the new specification.
		 * @param spec - The new specification.
		 */
		virtual void SetSpecification(const FramebufferSpecs& spec) = 0;

		/**
		 * Recreates the framebuffer.
		 */
		virtual void Recreate() = 0;

		/**
		 * Resizes the framebuffer and recreates it.
		 *This has no effect if the framebuffer cannot be resized (i.e. FramebufferSpecs::Resizable is set to false).
		 * @param width - The new width.
		 * @param height - The new height.
		 */
		virtual void Resize(unsigned int width, unsigned int height) = 0;

		/**
		 * Binds a textures for future use in shaders.
		 * @param id - The texture index in the list.
		 * @param slot - The texture slot to use.
		 */
		virtual void BindTexture(unsigned int id, unsigned int slot = 0) = 0;

		/**
		 * Binds all the textures for future use in shaders.
		 */
		virtual void BindAllTextures() = 0;

		/**
		 * Clears a texture.
		 * @param id - The texture index in the list.
		 * @param value - The color value to use.
		 */
		virtual void ClearAttachmentTexture(unsigned int id, unsigned int value) = 0;

		/**
		 * Clears all textures.
		 * @param value - The color value to use.
		 */
		virtual void ClearAllAttachmentTextures(unsigned int value) = 0;

		/**
		 * @returns The framebuffer specification.
		 */
		FramebufferSpecs GetSpec() const { return m_Specs; }

	protected:
		FramebufferSpecs m_Specs;

	private:
		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = -1;

		unsigned int m_ID = -1;
	};
}