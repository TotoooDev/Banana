#pragma once
#include <Core/Ref.h>
#include <Graphics/Texture.h>
#include <glm/glm.hpp>

namespace Banana
{
	/**
	 * Represents a material.
	 */
	struct Material
	{
		bool UseColors = false; /** If set to true, the RendererAPI will use ColorDiffuse and ColorSpecular to draw the object. */
		bool DrawWireframe = false; /** If set to true, the RendererAPI will draw the object in wireframe mode. */

		glm::vec3 ColorDiffuse = glm::vec3(0.0f);
		glm::vec3 ColorSpecular = glm::vec3(0.0f);

		Ref<Texture> TextureDiffuse;
		Ref<Texture> TextureSpecular;
	};
}