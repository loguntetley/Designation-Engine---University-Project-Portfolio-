#pragma once
#include "PCH.h"
#include "Core.h"


enum class EventType
{
	NoEvent,
	UpdateNormalEvent, UpdateFixedFramesEvent,
	KeyPressedEvent, KeyReleasedEvent, 
	MouseButtonEvent, MousePressedEvent, MouseReleasedEvent, MouseMoveEvent, MouseScrollEvent,
	WindowResizeEvent, WindowOpenEvent, WindowCloseEvent, WindowMoveEvent, WindowFocusEvent, WindowLostFocusEvent
};

enum EventCategory
{
	NoEventCategory,
	EventCategoryUpdate,
	EventCategoryWindow,
	EventCategoryInput,
	EventCategoryKeyboard,
	EventCategoryMouse,
};

#define EVENT_CLASS_TYPE(_eventType) static EventType GetStaticEventType() { return EventType::##_eventType; } \
                                      virtual const EventType GetEventType() const override { return GetStaticEventType(); } \
                                      virtual const std::string GetEventName() const override { return #_eventType; };

class DESIGNATION_API Event
{
private:
	friend class EventDispatcher;

protected:
	bool m_EventOngoing = false;

public:
	virtual const EventType GetEventType() const = 0;
	virtual const std::string GetEventName() const = 0; //For Debugging Only
	virtual const int GetEventCategory() const = 0;
	inline const bool CheckIsInCategory(EventCategory _eventCategory) const;

};

class DESIGNATION_API EventDispatcher
{
private:
	template<typename T>
	using EventFunction = std::function<bool(T&)>;

	Event& m_Event;

public:
	EventDispatcher (Event& _event) : m_Event(_event) {}

	template<typename T>
	bool Dispatch(EventFunction<T> _eventFunction);

};
