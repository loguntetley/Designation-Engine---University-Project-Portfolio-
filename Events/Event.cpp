#include "Event.h"

const bool Event::CheckIsInCategory(EventCategory _eventCategory) const 
{
	return GetEventCategory() & _eventCategory;
}

template<typename T>
bool EventDispatcher::Dispatch(EventFunction<T> _eventFunction)
{
	if (m_Event.GetEventType() == T::GetStaticEventType())
	{
		m_Event.m_EventOngoing = _eventFunction(*(T*)m_Event);
		return true;
	}
	return false;
}

