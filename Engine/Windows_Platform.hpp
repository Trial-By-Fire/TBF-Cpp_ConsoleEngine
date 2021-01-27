#pragma once



// Windows Library

#include <conio.h>
#include <Windows.h>

// Engine

#include "LAL.hpp"



namespace OSPlatform
{
	// Macros

	#define Console_Cursor_MinSize 1

	#define Console_Cursor_NotVisible 0


	#define Console_WhiteCell \
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY



	// Enums

	enum EKeyCode
	{
		Key_None        = 0x00,
		Key_Arrow_Up    = VK_UP,
		Key_Arrow_Down  = VK_DOWN,
		Key_Arrow_Left  = VK_LEFT,
		Key_Arrow_Right = VK_RIGHT,
		Key_Enter       = VK_RETURN,
		Key_Tab         = VK_TAB,
	};


	// Aliases (Typedefs)




	// Functions

	bool Bind_IOBufferTo_Console(void);

	bool RequestConsole(void);

	bool Unbind_IOBufferTo_Console(void);

	bool GetKeySignal(EKeyCode _key);
}

