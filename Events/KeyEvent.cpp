#include "KeyEvent.h"


inline int const KeyEvent::GetKeyCode() const { return m_KeyCode; }

const int KeyEvent::GetEventCategory() const { return EventCategoryKeyboard | EventCategoryInput; }


