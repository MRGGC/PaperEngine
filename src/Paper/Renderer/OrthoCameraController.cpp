#include "p_pch.h"
#include "OrthoCameraController.h"

#include "Paper/Core/Input.h"
#include "Paper/Core/KeyCodes.h"
#include "Paper/Core/MouseButtonCodes.h"

namespace Paper 
{
	OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Rotation(rotation),
		  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}
	
	void OrthoCameraController::OnUpdate(Timestep dt)
	{
		if (Input::IsKeyPressed(PAPER_KEY_A))
			m_CameraPos.x -= m_CameraTranslationSpeed * dt;
		else if (Input::IsKeyPressed(PAPER_KEY_D))
			m_CameraPos.x += m_CameraTranslationSpeed * dt;
		
		if (Input::IsKeyPressed(PAPER_KEY_W))
			m_CameraPos.y += m_CameraTranslationSpeed * dt;
		else if (Input::IsKeyPressed(PAPER_KEY_S))
			m_CameraPos.y -= m_CameraTranslationSpeed * dt;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(PAPER_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * dt;
			if (Input::IsKeyPressed(PAPER_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * dt;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPos);

		m_CameraTranslationSpeed = 2.0f * m_ZoomLevel;
	}
	
	void OrthoCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>(PAPER_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(PAPER_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		const float k = 0.25f;
		m_ZoomLevel -= e.GetOffsetY() * k;
		m_ZoomLevel = std::max(m_ZoomLevel, k);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float) e.GetWidth() / (float) e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}