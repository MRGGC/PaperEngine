#include "p_pch.h"

#include "Application.h"
#include "Log.h"
#include <Paper/Events/ApplicationEvent.h>


namespace Paper {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Running = true;
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}