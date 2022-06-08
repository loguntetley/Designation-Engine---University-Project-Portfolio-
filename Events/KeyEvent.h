#pragma once
#include "Event.h"
#include "PCH.h"

class DESIGNATION_API KeyEvent : public Event
{
protected:
	int m_KeyCode;
	inline int const GetKeyCode() const;

public:
	KeyEvent(int _keyCode) : m_KeyCode(_keyCode) {}

	const int GetEventCategory() const override;
};

class DESIGNATION_API KeyPressedEvent : public KeyEvent
{
private:
	bool m_KeyRepeated;

public:
	KeyPressedEvent(int _keyCode, bool _keyRepeated) : KeyEvent(_keyCode), m_KeyRepeated(_keyRepeated) {}

	EVENT_CLASS_TYPE(KeyPressedEvent);

};

class DESIGNATION_API KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int _keyCode) : KeyEvent(_keyCode) {}

	EVENT_CLASS_TYPE(KeyReleasedEvent);

};
