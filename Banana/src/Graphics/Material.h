#pragma once
#include <Core/Ref.h>
#include <Graphics/Texture.h>
#include <glm/glm.hpp>

namespace Banana
{
	struct Material
	{
		bool UseColors = false;
		bool DrawWireframe = false;

		glm::vec3 ColorAmbient = glm::vec3(0.0f);
		glm::vec3 ColorDiffuse = glm::vec3(0.0f);
		glm::vec3 ColorSpecular = glm::vec3(0.0f);

		Ref<Texture> TextureAmbient;
		Ref<Texture> TextureDiffuse;
		Ref<Texture> TextureSpecular;
	};
}