#pragma once
#include <glm/glm.hpp>

namespace Banana
{
	struct DirectionalLight
	{
		glm::vec3 Direction = glm::vec3(0.0f, -1.0f, 0.0f);

		glm::vec3 Ambient = glm::vec3(0.2f);
		glm::vec3 Diffuse = glm::vec3(0.5f);
		glm::vec3 Specular = glm::vec3(1.0f);
	};

	struct PointLight
	{
		float Constant = 1.0f;
		float Linear = 0.027f;
		float Quadratic = 0.0028f;

		glm::vec3 Ambient = glm::vec3(0.2f);
		glm::vec3 Diffuse = glm::vec3(0.5f);
		glm::vec3 Specular = glm::vec3(1.0f);
	};

	struct SpotLight
	{
		glm::vec3 Direction = glm::vec3(0.0f, -1.0f, 0.0f);

		float Constant = 1.0f;
		float Linear = 0.027f;
		float Quadratic = 0.0028f;

		float CutOff = glm::cos(glm::radians(12.5f));
		float OuterCutOff = glm::cos(glm::radians(17.5f));

		glm::vec3 Ambient = glm::vec3(0.2f);
		glm::vec3 Diffuse = glm::vec3(0.5f);
		glm::vec3 Specular = glm::vec3(1.0f);
	};
}