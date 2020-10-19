#pragma once

#include "p_pch.h"

#include "Paper/Core/Core.h"
#include "Paper/Events/Event.h"

namespace Paper
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Paper Engine", unsigned int w = 1280, unsigned int h = 720)
			: Title(title), Width(w), Height(h) {}
	};
	

	// Window Abstraction for Desktop apps
	class PAPER_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const  = 0;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}