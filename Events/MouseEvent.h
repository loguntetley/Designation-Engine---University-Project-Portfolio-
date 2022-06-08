#pragma once
#include "Event.h"
#include "PCH.h"


class DESIGNATION_API MouseEvent : public Event
{
protected:
	const int GetEventCategory() const override;

};

class DESIGNATION_API MouseButtonEvent : public MouseEvent
{
private:
	int m_Button;

public:
	MouseButtonEvent(int _button) : m_Button(_button) {}

	inline const float GetMouseButton() const;

	EVENT_CLASS_TYPE(MouseButtonEvent);

};

class DESIGNATION_API MousePressedEvent : public MouseButtonEvent
{
public:
	MousePressedEvent(int _button) : MouseButtonEvent(_button) {};

	EVENT_CLASS_TYPE(MousePressedEvent);
};

class DESIGNATION_API MouseReleasedEvent : public MouseButtonEvent
{
public:
	MouseReleasedEvent(int _button) : MouseButtonEvent(_button) {};

	EVENT_CLASS_TYPE(MouseReleasedEvent);

};

class DESIGNATION_API MouseMoveEvent : public MouseEvent
{
private:
	float m_XPos, m_YPos;

public:
	MouseMoveEvent(float _xPos, float _yPos) : m_XPos(_xPos), m_YPos(_yPos) {};

	inline const float GetMouseXPos() const;
	inline const float GetMouseYPos() const;

	EVENT_CLASS_TYPE(MouseMoveEvent);
};

class DESIGNATION_API MouseScrollEvent : public MouseEvent
{
private:
	float m_XOffset, m_YOffset;

public:
	MouseScrollEvent(float _xOffset, float _yOffset) : m_XOffset(_xOffset), m_YOffset(_yOffset) {};

	inline const float GetMouseXOffset() const;
	inline const float GetMouseYOffset() const;

	EVENT_CLASS_TYPE(MouseScrollEvent);

};
