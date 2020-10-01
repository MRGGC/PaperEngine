#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
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

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = false;
	private:
		static Application* s_Instance;
	};

	// CLIENT PART
	PAPER_API Application* CreateApplication();
}