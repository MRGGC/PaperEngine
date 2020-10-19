#pragma once

#include "Paper/Core/Core.h"
#include "Paper/Core/Window.h"
#include "Paper/Core/LayerStack.h"
#include "Paper/ImGui/ImGuiLayer.h"
#include "Paper/Events/ApplicationEvent.h"

#include "Paper/Core/Timestep.h"

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
		bool OnWindowResize(WindowResizeEvent& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		bool m_Mimized = false;

		float m_LastFrameTime = 0.0f;
		bool m_Running = false;
	private:
		static Application* s_Instance;
	};

	// CLIENT PART
	PAPER_API Application* CreateApplication();
}