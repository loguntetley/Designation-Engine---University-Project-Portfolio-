#pragma once
#include "PCH.h"

#include "Core.h"
#include "Log.h"

class DESIGNATION_API Logger
{
private:
	static std::shared_ptr<Log> m_CoreLogger;
	static std::shared_ptr<Log> m_ClientLogger;

public:
	inline static std::shared_ptr<Log>& GetCoreLogger();
	inline static std::shared_ptr<Log>& GetClientLogger();

};

//Core logging macros
#define DN_CORE_ERROR(...) Logger::GetCoreLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelError)
#define DN_CORE_WARNING(...) Logger::GetCoreLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelWarning)
#define DN_CORE_SUCCESS(...) Logger::GetCoreLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelSuccess)
#define DN_CORE_INFO(...) Logger::GetCoreLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelInfo)
#define DN_CORE_IMPORTANT_INFO(...) Logger::GetCoreLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelImportantInfo)

#define DN_CORE_ERROR_OUTPUT(...) Logger::GetCoreLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelError)
#define DN_CORE_WARNING_OUTPUT(...) Logger::GetCoreLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelWarning)
#define DN_CORE_SUCCESS_OUTPUT(...) Logger::GetCoreLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelSuccess)
#define DN_CORE_INFO_OUTPUT(...) Logger::GetCoreLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelInfo)
#define DN_CORE_IMPORTANT_INFO_OUTPUT(...) Logger::GetCoreLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelImportantInfo)

//Client logging macros
#define DN_ERROR(...) Logger::GetClientLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelError)
#define DN_WARNING(...) Logger::GetClientLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelWarning)
#define DN_SUCCESS(...) Logger::GetClientLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelSuccess)
#define DN_INFO(...) Logger::GetClientLogger()->AddLog(__VA_ARGS__, Log::WarningLevel::LevelInfo)

#define DN_ERROR_OUTPUT(...) Logger::GetClientLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelError)
#define DN_WARNING_OUTPUT(...) Logger::GetClientLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelWarning)
#define DN_SUCCESS_OUTPUT(...) Logger::GetClientLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelSuccess)
#define DN_INFO_OUTPUT(...) Logger::GetClientLogger()->OutputLog(__VA_ARGS__, Log::WarningLevel::LevelInfo)



