#pragma once

#include "Core.h"
#include "Window.h"
#include <Paper/Events/ApplicationEvent.h>

namespace Paper
{
	class PAPER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;
	};

	// CLIENT PART
	PAPER_API Application* CreateApplication();
}