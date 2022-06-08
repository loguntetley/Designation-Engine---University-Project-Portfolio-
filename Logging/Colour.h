#pragma once
#include "PCH.h"

//COLOUR DEFINES//
#define BLUE (int(1))
#define GREEN (int(2))
#define LIGHT_BLUE (int(3))
#define RED (int(4))
#define PURPLE (int(5))
#define YELLOW (int(6))
#define DARK_WHITE (int 7))
#define GREY (int(8))
#define MARINER (int(9))
#define LIME (int(10))
#define CYAN (int(11))
#define CRIMSON (int(12))
#define VIOLET (int(13))
#define BEIGE (int(14))
#define WHITE (int(15))

const class Colour
{
private:
	const static HANDLE m_ConsoleHandle;

public:
	const static void SetColour(const unsigned int _colour);
	const static void ClearColour();

};

