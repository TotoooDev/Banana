#pragma once
#include <glm/glm.hpp>

namespace Banana
{
	class Camera
	{
	public:
		Camera(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float roll = 0.0f);

		glm::mat4 GetViewMatrix(glm::vec3 pos);
		void UpdateCameraVectors();

		// glm::vec3 Pos;
		float Yaw;
		float Pitch;
		float Roll;


	private:
		glm::vec3 m_Front;
		glm::vec3 m_Right;
		glm::vec3 m_Up;
		glm::vec3 m_WorldUp;
	};
}