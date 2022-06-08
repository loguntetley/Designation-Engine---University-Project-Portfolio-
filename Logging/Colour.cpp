#include "Colour.h"

const HANDLE Colour::m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

const void Colour::SetColour(const unsigned int _colour)
{
    SetConsoleTextAttribute(m_ConsoleHandle, _colour);
}

const void Colour::ClearColour()
{
    SetConsoleTextAttribute(m_ConsoleHandle, WHITE);
}
