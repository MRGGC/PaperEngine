#pragma once

#include "Event.h"

namespace Paper
{
	class PAPER_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int w, unsigned int h)
			: m_Width(w), m_Height(h) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: W=" << m_Width << ", H=" << m_Height;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategory::cApplication)
		EVENT_CLASS_TYPE(WindowResize)
	private:
		unsigned int m_Width, m_Height;
	};

	class PAPER_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_CATEGORY(EventCategory::cApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class PAPER_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_CATEGORY(EventCategory::cApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};

	class PAPER_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_CATEGORY(EventCategory::cApplication)
		EVENT_CLASS_TYPE(AppRender)
	};
}