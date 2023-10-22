#pragma once
#include <glm/glm.hpp>

namespace Banana
{
	/**
	 * A directionnal light. Use with a TransformComponent to set its rotation.
	 * See https://learnopengl.com/Lighting/Light-casters for more details about lights.
	 */
	struct DirectionalLight
	{
		glm::vec3 Ambient = glm::vec3(0.2f); /** The ambient color */
		glm::vec3 Diffuse = glm::vec3(0.5f); /** The diffuse color */
		glm::vec3 Specular = glm::vec3(1.0f); /** The specular color */
	};

	/**
	 * A point light. Use with a TransformComponent to set its position.
	 * See https://learnopengl.com/Lighting/Light-casters for more details about lights.
	 */
	struct PointLight
	{
		glm::vec3 Ambient = glm::vec3(0.2f); /** The ambient color */
		glm::vec3 Diffuse = glm::vec3(0.5f); /** The diffuse color */
		glm::vec3 Specular = glm::vec3(1.0f); /** The specular color */

		float Constant = 1.0f;
		float Linear = 0.14f;
		float Quadratic = 0.07f;
	};
	
	/**
	 * A spot light. Use with a TransformComponent to set its position and rotation.
	 * See https://learnopengl.com/Lighting/Light-casters for more details about lights.
	 */
	struct SpotLight
	{
		glm::vec3 Ambient = glm::vec3(0.2f); /** The ambient color */
		glm::vec3 Diffuse = glm::vec3(0.5f); /** The diffuse color */
		glm::vec3 Specular = glm::vec3(1.0f); /** The specular color */

		float Constant = 1.0f;
		float Linear = 0.14f;
		float Quadratic = 0.07f;

		float CutOff = 12.5f;
		float OuterCutOff = 17.5f;
	};
}