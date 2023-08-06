#pragma once
#include <string>

namespace Banana
{
	enum class TextureType
	{
		Diffuse,
		Specular,
		Normal
	};

	class Texture
	{
	public:
		Texture(bool flipUVs = false);
		Texture(const std::string& path, bool flipUVs = false);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		unsigned int GetID() const;

	private:
		void CreateFromFile(const std::string& path);
	
	private:
		unsigned int m_ID;
	};
}