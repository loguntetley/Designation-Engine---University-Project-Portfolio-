#include "MouseEvent.h"

const int MouseEvent::GetEventCategory() const { return EventCategoryMouse | EventCategoryInput; }

inline const float MouseButtonEvent::GetMouseButton() const { return m_Button; }

inline const float MouseMoveEvent::GetMouseXPos() const { return m_XPos; }

inline const float MouseMoveEvent::GetMouseYPos() const { return m_YPos; }

inline const float MouseScrollEvent::GetMouseXOffset() const { return m_XOffset; }

inline const float MouseScrollEvent::GetMouseYOffset() const { return m_YOffset; }
	
