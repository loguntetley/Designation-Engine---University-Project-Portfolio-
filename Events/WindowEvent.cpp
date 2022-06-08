#include "WindowEvent.h"

const int WindowEvent::GetEventCategory() const { return EventCategoryWindow; }

inline const float WindowResizeEvent::GetWindowWidth() const { return m_WindowWidth; }

inline const float WindowResizeEvent::GetWindowHight() const { return m_WindowHight; }

inline const float WindowOpenEvent::GetWindowOpen() const { return m_WindowOpen; }

inline const float WindowCloseEvent::GetWindowClose() const { return m_WindowClose; }

inline const float WindowMoveEvent::GetWindowXPos() const { return m_WindowXPos; }

inline const float WindowMoveEvent::GetWindowYPos() const { return m_WindowYPos; }

inline const float WindowFocusEvent::GetWindowFocus() const { return m_WindowFocus; }

inline const float WindowLostFocusEvent::GetWindowLostFocus() const { return m_WindowLostFocus; }
