#include "p_pch.h"

#include "Paper/Core/Application.h"
#include "Paper/Core/Log.h"
#include "Paper/Core/Input.h"
#include "Paper/Events/ApplicationEvent.h"

#include "Paper/Renderer/Renderer.h"

#include <GLFW/glfw3.h> // Temporary: Until Tying Time to the Platform

namespace Paper {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		PAPER_PROFILE_FUNCTION();

		// TODO: Only 1 instance of application is possible right now. Change it!
		PAPER_CORE_ASSERT(!s_Instance, "Paper Application already Exists!");
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(PAPER_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Running = true;
	}

	Application::~Application()
	{
		PAPER_PROFILE_FUNCTION();
	}

	void Application::PushLayer(Layer* layer)
	{
		PAPER_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		PAPER_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		PAPER_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PAPER_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(PAPER_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		PAPER_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Mimized = true;
			return false;
		}

		m_Mimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::Run()
	{
		PAPER_PROFILE_FUNCTION();

		while (m_Running)
		{
			PAPER_PROFILE_SCOPE("Run Loop");

			float time = glfwGetTime(); // Temporary: Until Tying Time to the Platform
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Mimized) {
				{
					PAPER_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			{
				PAPER_PROFILE_SCOPE("LayerStack ImGuiRenderer");
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRenderer();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}