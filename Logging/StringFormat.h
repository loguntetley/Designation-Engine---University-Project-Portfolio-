#pragma once
#include "PCH.h"

class StringFormat
{
private:
	static std::string GetTime();

public:
	static std::string FormatStringLog(const std::string& _message);

};