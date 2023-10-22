#pragma once
#include <Core/Ref.h>
#include <string>

namespace Banana
{
	/**
	 * Represents the texture wrapping mode.
	 */
	enum class TextureWrapping
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	/**
	 * Represents the texture filtering mode.
	 */
	enum class TextureFiltering
	{
		Nearest,
		Linear
	};

	/**
	 * Represents the number of channels for the texture.
	 */
	enum class TextureChannels
	{
		R,
		RG,
		RGB,
		RGBA
	};

	/**
	 * The specification for a texture.
	 */
	struct TextureSpecs
	{
		TextureWrapping WrappingMode = TextureWrapping::ClampToBorder;
		TextureFiltering FilteringMode = TextureFiltering::Nearest;
		TextureChannels Channels = TextureChannels::RGB; /** Only used if Width and Height are not -1 */
		/** 
		 * These are only used when creating an empty texture so that OpenGL knows what size the texture needs to be.
		 * Leave them to -1 or any negative number if you want to load a texture from a file.
		 */
		int Width = -1, Height = -1;
	};

	/** 
	 * Provides an API for 2D textures.
	 */
	class Texture
	{
	public:
		/**
		 * Creates an empty texture.
		 * @returns The newly created texture.
		 * @param specs - The texture specification to use.
		 */
		static Ref<Texture> Create(const TextureSpecs& specs);
		/**
		 * Creates an texture from a file.
		 * @returns The newly created texture.
		 * @param specs - The texture specification to use. 
		 * @param path - The file path of the texture image. 
		 * @param flip - If set to true, flips the image vertically. 
		 */
		static Ref<Texture> Create(const TextureSpecs& specs, const std::string& path, bool flip = false);

		/**
		 * Loads the texture from a file.
		 * @param path - The file path of the image.
		 * @param flip - If set to true, flips the image vertically.
		 */
		virtual void LoadFromFile(const std::string& path, bool flip = false) = 0;

		/**
		 * Binds the texture for future uses.
		 */
		virtual void Bind(unsigned int unit = 0) = 0;

		/**
		 * @returns The internal ID of the texture.
		 */
		virtual unsigned int GetID() = 0;

	protected:
		TextureSpecs m_Specs;
	};
}