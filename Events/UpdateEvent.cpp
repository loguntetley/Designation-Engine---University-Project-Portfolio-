#include "UpdateEvent.h"

const int UpdateEvent::GetEventCategory() const { return EventCategoryUpdate; }

inline const unsigned int UpdateFixedFramesEvent::GetFrames() const { return m_Frames; }


