#pragma once
#include "Event.h"
#include "PCH.h"

class DESIGNATION_API UpdateEvent : public Event
{
public:
	const int GetEventCategory() const override;
};

class DESIGNATION_API UpdateNoramlEvent : UpdateEvent
{
public:
	UpdateNoramlEvent() {};

	EVENT_CLASS_TYPE(UpdateNormalEvent);
};

class DESIGNATION_API UpdateFixedFramesEvent : UpdateEvent
{
private:
	unsigned int m_Frames;

public:
	UpdateFixedFramesEvent(unsigned int _frames) : m_Frames(_frames) {};

	inline const unsigned int GetFrames() const;

	EVENT_CLASS_TYPE(UpdateFixedFramesEvent);
};


