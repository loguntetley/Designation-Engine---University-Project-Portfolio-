#pragma once
#include "PCH.h"

#include "Core.h"

class Log
{
public:
	enum WarningLevel : int
	{
		LevelError = 4,
		LevelWarning = 6,
		LevelInfo = 15,
		LevelSuccess = 2,
		LevelImportantInfo = 1
	};

	typedef void(Log::* outputFunction)(const std::string& _message, const WarningLevel& _warningLevel);
	std::queue<outputFunction> m_LogQueue;
	std::queue<std::tuple<std::string, Log::WarningLevel>> m_LogDataQueue;
	std::thread m_Thread{ &Log::Serialisation, this };

	void Serialisation();
	

	template<class T>
	Log& operator <<(const T& _value);							//To log normal value
	Log& operator <<(std::ios& (*_pfn)(std::ios&));				//Overload for ios manipulator
	Log& operator <<(std::ios_base& (*_pfn)(std::ios_base&));	//Overload for ios_base manipulator
	Log& operator <<(std::ostream& (*_pfn)(std::ostream&));		//Overload for ostream manipulator

public:
	Log();
	~Log();
	Log(const Log&) = delete;									// non construction-copyable
	Log& operator=(const Log&) = delete;						// non copyable

	void AddLog(const std::string& _message, const WarningLevel& _warningLevel);
	void AddLog(const char& _message, const WarningLevel& _warningLevel);
	void AddLog(const int& _message, const WarningLevel& _warningLevel);
	void AddLog(const float& _message, const WarningLevel& _warningLevel);

	void OutputLog(const std::string& _message, const WarningLevel& _warningLevel);

};
