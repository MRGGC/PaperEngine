#pragma once

#include "Paper/Core.h"

namespace Paper
{

	/*
	 * Events are blocking type yet.
	 * Whenever an event occurs, the whole applications stops
	 * and does the event. After finishing, the application continues
	 * TODO: Create Buffered/Queued Event System
	 */

	enum class EventType
	{
		None=0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender, // ?Remove
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		cNone = 0,
		cApplication   = BIT(0),
		cInput         = BIT(1),
		cKeyboard      = BIT(2),
		cMouse         = BIT(3),
		cMouseButton   = BIT(4)
	};

	#define EVENT_CLASS_CATEGORY(category) int GetCategoryFlags() const override { return category; }

	#if defined(_MSC_VER)
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
									virtual EventType GetEventType() const override { return GetStaticType(); } \
									virtual const char* GetName() const override { return #type; }
	#else
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
									virtual EventType GetEventType() const override { return GetStaticType(); } \
									virtual const char* GetName() const override { return #type; }
	#endif

	class PAPER_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0; // ?Remove
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};

	class PAPER_API EventDispatcher
	{
	template<typename T>
	using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& e)
			: m_Event(e) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		os << e.ToString();
		return os;
	}
}