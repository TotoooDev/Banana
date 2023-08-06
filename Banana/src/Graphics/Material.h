#pragma once
#include <Core/Ref.h>
#include <Graphics/Texture.h>
#include <glm/glm.hpp>

namespace Banana
{
	struct Material
	{
		glm::vec3 ColorAmbient;
		glm::vec3 ColorDiffuse;
		glm::vec3 ColorSpecular;

		Ref<Texture> Diffuse = CreateRef<Texture>();
		Ref<Texture> Specular = CreateRef<Texture>();
		Ref<Texture> Normal = CreateRef<Texture>();
		float Shininess = 32.0f;

		std::string Name = "";
	};
}