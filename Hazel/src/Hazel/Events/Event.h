#pragma once

#include "hzprh.h"
#include "Hazel/Core.h"

// #include <functional>
// #include <sstream>

namespace Hazel
{
	// Events are dealt immediately after an event occurs
	// A better option is using event buffer

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication		= BIT(0),
		EventCategoryInput				= BIT(1),
		EventCategoryKeyboard			= BIT(2),
		EventCategoryMouseButton		= BIT(3),
		EventCategoryMouse				= BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
								virtual EventType GetEventType() const override { return GetStaticType(); } \
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

	protected:
		bool m_Handled = false;
	};

	
	class HAZEL_API EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& e)
			: m_Event(e) 
		{
		}

		template <typename T>
		bool Dispatch(EventFn<bool(T&)> func)
		{
			if (m.Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			else
				return false;
		}

	private:
		Event &m_Event;
	};

	// There is no support this feature in the newer version of spdlog anymore
	// inline std::ostream& operator<<(std::ostream& os, const Event& e)
	// {
	// 	return os << e.ToString().c_str();
	// }
}