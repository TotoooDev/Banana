#pragma once
#include <Core/Ref.h>
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
		TextureWrapping WrappingMode = TextureWrapping::ClampToBorder;
		TextureFiltering FilteringMode = TextureFiltering::Nearest;
		// Only used if Width and Height are not -1
		TextureChannels Channels = TextureChannels::RGB;
		// These are only used when creating an empty texture so that OpenGL knows what size the texture needs to be.
		// Leave them to -1 or any negative number if you want to load a texture from a file.
		int Width = -1, Height = -1;
	};

	// Wrapper for 2D textures
	class Texture
	{
	public:
		static Ref<Texture> Create(const TextureSpecs& specs);
		static Ref<Texture> Create(const TextureSpecs& specs, const std::string& path, bool flip = false);

		virtual void LoadFromFile(const std::string& path, bool flip = false) = 0;

		virtual void Bind(unsigned int unit = 0) = 0;

	protected:
		TextureSpecs m_Specs;
	};
}