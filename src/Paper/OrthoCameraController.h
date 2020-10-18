#pragma once

#include <glm/glm.hpp>

#include "Paper/Renderer/OrthographicCamera.h"
#include "Paper/Core/Timestep.h"

#include "Paper/Events/ApplicationEvent.h"
#include "Paper/Events/MouseEvent.h"

namespace Paper
{
	class OrthoCameraController
	{
	public:
		OrthoCameraController(float aspectRatio, bool rotation = false);
		
		void OnUpdate(Timestep dt);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 4.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}