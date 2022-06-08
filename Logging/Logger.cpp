#include "Logger.h"

std::shared_ptr<Log> Logger::m_CoreLogger = std::make_shared<Log>();
std::shared_ptr<Log> Logger::m_ClientLogger = std::make_shared<Log>();;

inline std::shared_ptr<Log>& Logger::GetCoreLogger()
{
	return m_CoreLogger;
}

inline std::shared_ptr<Log>& Logger::GetClientLogger()
{
	return m_ClientLogger;
}
