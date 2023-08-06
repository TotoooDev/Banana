#include <Graphics/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Banana
{
	Camera::Camera(glm::vec3 up, float yaw, float pitch, float roll)
		: m_WorldUp(up), Yaw(yaw), Pitch(pitch), Roll(roll)
	{
		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix(glm::vec3 pos)
	{
		return glm::lookAt(pos, pos + m_Front, m_Up);
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		m_Front = glm::normalize(front);

		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		glm::mat4 rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), m_Front);
		m_Up = glm::mat3(rollMat) * m_Up;
	}
}