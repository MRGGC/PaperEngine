#include "p_pch.h"

#include "Paper/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Paper
{
	OrthographicCamera::OrthographicCamera(float l, float r, float b, float t)
		: m_ProjectionMatrix(glm::ortho(l, r, b, t, -1.0f, 1.0f)), m_ViewMatrix(glm::mat4(1.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float l, float r, float b, float t)
	{
		m_ProjectionMatrix = glm::ortho(l, r, b, t, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateVP()
	{
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}