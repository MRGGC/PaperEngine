#pragma once

#include "Event.h"

#include <sstream>

namespace Paper
{
	class PAPER_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float mouseX, float mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_MouseX << ", " << m_MouseY << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategory::cMouse | EventCategory::cInput)
		EVENT_CLASS_TYPE(MouseMoved)
	private:
		float m_MouseX;
		float m_MouseY;
	};

	class PAPER_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offx, float offy)
			: m_OffsetX(offx), m_OffsetY(offy) {}

		inline float GetOffsetX() const { return m_OffsetX; }
		inline float GetOffsetY() const { return m_OffsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_OffsetX << ", " << m_OffsetY << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategory::cMouse | EventCategory::cInput)
		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		float m_OffsetX;
		float m_OffsetY;
	};

	class PAPER_API MouseButtonEvent : public Event
	{
	public:
		inline int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategory::cMouse | EventCategory::cInput)
	protected:
		int m_Button;
		MouseButtonEvent(int btn) : m_Button(btn) {}
	};

	class PAPER_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int btn) : MouseButtonEvent(btn) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class PAPER_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
		MouseButtonReleasedEvent(int btn) : MouseButtonEvent(btn) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}