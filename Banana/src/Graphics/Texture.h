#pragma once
#include <string>

namespace Banana
{
	enum class TextureWrapping
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFiltering
	{
		Nearest,
		Linear
	};

	enum class TextureChannels
	{
		R,
		RG,
		RGB,
		RGBA
	};

	struct TextureSpecs
	{
		TextureWrapping WrappingMode;
		TextureFiltering FilteringMode;
		// Only used if Width and Height are not -1
		TextureChannels Channels;
		// These are only used when creating an empty texture so that OpenGL knows what size the texture needs to be.
		// Leave them to -1 or any negative number if you want to load a texture from a file.
		int Width, Height;

		TextureSpecs(TextureWrapping wrapping, TextureFiltering filtering, TextureChannels channels = TextureChannels::RGB, int width = -1, int height = -1)
			: WrappingMode(wrapping), FilteringMode(filtering), Channels(channels), Width(width), Height(height) {}
	};

	// Wrapper for 2D textures
	class Texture
	{
	public:
		Texture(TextureSpecs specs);
		~Texture();

		void LoadFromFile(const std::string& path, bool flip = false);

		void Bind(unsigned int unit = 0);

	private:
		TextureSpecs m_Specs;

		unsigned int m_ID;
	};
}