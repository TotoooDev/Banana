#pragma once
#include <Core/Application.h>
#include <Events/Events.h>

using namespace Banana;

struct OrbitingCamera
{
	void SetCenter(glm::vec3 center) { m_Center = center; }
	void SetRadius(float radius) { m_Radius = radius; }

	glm::vec3 GetCenter() { return m_Center; }
	float GetRadius() { return m_Radius; }
	float GetAzimuth() { return m_Azimuth; }
	float GetPolar() { return m_Polar; }

	void RotateAzimuth(float angle)
	{
		m_Azimuth += angle;

		float fullCircle = 2.0f * glm::pi<float>();
		m_Azimuth = glm::mod(m_Azimuth, fullCircle);
		if (m_Azimuth < 0.0f)
			m_Azimuth += fullCircle;
	}

	void RotatePolar(float angle)
	{
		m_Polar += angle;

		float polarCap = glm::pi<float>() / 2.0f;
		if (m_Polar > polarCap)
			m_Polar = polarCap;
		if (m_Polar < -polarCap)
			m_Polar = -polarCap;
	}

	void Zoom(float zoom)
	{
		m_Radius -= zoom;
		if (m_Radius < m_MinRadius)
			m_Radius = m_MinRadius;
	}

	glm::vec3 GetPosCartesian()
	{
		float sinAzimuth = glm::sin(m_Azimuth);
		float cosAzimuth = glm::cos(m_Azimuth);
		float sinPolar = glm::sin(m_Polar);
		float cosPolar = glm::cos(m_Polar);

		float x = m_Center.x + m_Radius * cosPolar * cosAzimuth;
		float y = m_Center.y + m_Radius * sinPolar;
		float z = m_Center.z + m_Radius * cosPolar * sinAzimuth;

		return glm::vec3(x, y, z);
	}

private:
	glm::vec3 m_Center = glm::vec3(0.0f);
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_Radius = 10.0f;
	float m_MinRadius = 1.0f;
	float m_Azimuth = 0.0f;
	float m_Polar = 0.0f;
};