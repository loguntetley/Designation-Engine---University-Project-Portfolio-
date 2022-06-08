#pragma once
#include "Event.h"
#include "PCH.h"

class DESIGNATION_API WindowEvent : public Event
{
public:
	const int GetEventCategory() const override;

};

class DESIGNATION_API WindowResizeEvent : public WindowEvent
{
private:
	float m_WindowWidth, m_WindowHight;

public:
	WindowResizeEvent(float _windowWidth, float _windowHight) : m_WindowWidth(_windowWidth), m_WindowHight(_windowHight) {}

	inline const float GetWindowWidth() const;
	inline const float GetWindowHight() const;

	EVENT_CLASS_TYPE(WindowResizeEvent);

};

class DESIGNATION_API WindowOpenEvent : public WindowEvent
{
private:
	bool m_WindowOpen;

public:
	WindowOpenEvent(bool _windowOpen) : m_WindowOpen(_windowOpen) {}

	inline const float GetWindowOpen() const;

	EVENT_CLASS_TYPE(WindowOpenEvent);

};

class DESIGNATION_API WindowCloseEvent : public WindowEvent
{
private:
	bool m_WindowClose;

public:
	WindowCloseEvent(bool _windowClose) : m_WindowClose(_windowClose) {}

	inline const float GetWindowClose() const;

	EVENT_CLASS_TYPE(WindowCloseEvent);

};

class DESIGNATION_API WindowMoveEvent : public WindowEvent
{
private:
	float m_WindowXPos, m_WindowYPos;

public:
	WindowMoveEvent(float _windowXPos, float _windowYPos) : m_WindowXPos(_windowXPos), m_WindowYPos(_windowYPos) {}

	inline const float GetWindowXPos() const;
	inline const float GetWindowYPos() const;

	EVENT_CLASS_TYPE(WindowMoveEvent);

};

class DESIGNATION_API WindowFocusEvent : public WindowEvent
{
private:
	bool m_WindowFocus;

public:
	WindowFocusEvent(bool _windowFocus) : m_WindowFocus(_windowFocus) {}

	inline const float GetWindowFocus() const;

	EVENT_CLASS_TYPE(WindowFocusEvent);

};

class DESIGNATION_API WindowLostFocusEvent : public WindowEvent
{
private:
	bool m_WindowLostFocus;

public:
	WindowLostFocusEvent(bool _windowLostFocus) : m_WindowLostFocus(_windowLostFocus) {}

	inline const float GetWindowLostFocus() const;

	EVENT_CLASS_TYPE(WindowLostFocusEvent);

};
