#pragma once
#include <glm/glm.hpp>

namespace Banana
{
	/**
	 * A class to represent a perspective camera.
	 * This implementation is mostly based on the LearnOpenGL one with little to no modification (stealing code is my passion) available at https://learnopengl.com/Getting-started/Camera.
	 */
	class Camera
	{
	public:
		/**
		 * @param up - The vector that points upwards.
		 * @param yaw - The starting yaw angle.
		 * @param pitch - The starting pitch angle.
		 * @param roll - The starting roll angle.
		 */
		Camera(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float roll = 0.0f);

		/**
		 * @returns The view matrix of the camera.
		 */
		glm::mat4 GetViewMatrix(glm::vec3 pos);
		/**
		 * Updates the camera vectors. Call this method everytime you change one of the parameters to update all the vectors accordingly.
		 */
		void UpdateCameraVectors();

	// Getters
	public:
		/**
		 * @returns The front vector.
		 */
		glm::vec3 GetFrontVector() { return m_Front; }
		/**
		 * @returns The right vector.
		 */
		glm::vec3 GetRightVector() { return m_Right; }
		/**
		 * @returns The up vector.
		 */
		glm::vec3 GetUpVector() { return m_Up; }

	public:
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