#include "StringFormat.h"

std::string StringFormat::GetTime()
{
	time_t currentTime = time(NULL);
	char str[26];
	ctime_s(str, sizeof(str), &currentTime);
	return str;
}

std::string StringFormat::FormatStringLog(const std::string& _message)
{
	std::string time = GetTime();
	time.erase(std::remove(time.begin(), time.end(), '\n'), time.end());
	std::string formattedString = "[" + time + "] Designation: " + _message;
	return formattedString;
}

