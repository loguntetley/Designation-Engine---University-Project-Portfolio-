#include "Log.h"
#include "Colour.h"
#include "StringFormat.h"

#pragma region Constructors

Log::Log()
{
}

Log::~Log()
{
	m_Thread.join();
}

#pragma endregion

#pragma region Serialisation Functions

void Log::Serialisation()
{
	while (!m_LogQueue.empty())
	{
		std::tuple<std::string, Log::WarningLevel> logData = m_LogDataQueue.front();
		(this->*m_LogQueue.front())(std::get<0>(logData), std::get<1>(logData));
		m_LogQueue.pop();
		m_LogDataQueue.pop();
	}
}

void Log::OutputLog(const std::string& _message, const WarningLevel& _warningLevel)
{
	Colour::SetColour(_warningLevel);
	Log log;
	log << StringFormat::FormatStringLog(_message) << std::endl;
	Colour::ClearColour();
}

#pragma endregion

#pragma region Operator Overload Functions

template<class T>
inline Log& Log::operator<<(const T& _value)
{
	std::cout << _value;
	return *this;
}

Log& Log::operator<<(std::ios& (*_pfn)(std::ios&))
{
	std::cout << _pfn;
	return *this;
}

Log& Log::operator<<(std::ios_base& (*_pfn)(std::ios_base&))
{
	std::cout << _pfn;
	return *this;
}

Log& Log::operator<<(std::ostream& (*_pfn)(std::ostream&))
{
	std::cout << _pfn;
	return *this;
}

#pragma endregion

#pragma region Add Log Functions

void Log::AddLog(const std::string& _message, const WarningLevel& _warningLevel)
{
	std::tuple<std::string, Log::WarningLevel> logData{ _message, _warningLevel };
	m_LogDataQueue.emplace(logData);
	outputFunction function = &Log::OutputLog;
	m_LogQueue.emplace(function);
}

void Log::AddLog(const char& _message, const WarningLevel& _warningLevel)
{
	std::tuple<std::string, Log::WarningLevel> logData{ std::string(1, _message), _warningLevel };
	m_LogDataQueue.emplace(logData);
	outputFunction function = &Log::OutputLog;
	m_LogQueue.emplace(function);
}

void Log::AddLog(const int& _message, const WarningLevel& _warningLevel)
{
	std::tuple<std::string, Log::WarningLevel> logData{ std::to_string(_message), _warningLevel };
	m_LogDataQueue.emplace(logData);
	outputFunction function = &Log::OutputLog;
	m_LogQueue.emplace(function);
}

void Log::AddLog(const float& _message, const WarningLevel& _warningLevel)
{
	std::tuple<std::string, Log::WarningLevel> logData{ std::to_string(_message), _warningLevel };
	m_LogDataQueue.emplace(logData);
	outputFunction function = &Log::OutputLog;
	m_LogQueue.emplace(function);
}

#pragma endregion
