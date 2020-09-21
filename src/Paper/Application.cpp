#include "p_pch.h"

#include "Application.h"
#include "Log.h"
#include <Paper/Events/ApplicationEvent.h>


namespace Paper {
	
	bool test(WindowResizeEvent& wre)
	{
		PAPER_CLIENT_TRACE(wre);
		return true;
	}

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(100, 120);

		EventDispatcher ed(e);
		ed.Dispatch<WindowResizeEvent>(test);

		while (true);
	}
}