#pragma once



// Windows Library

#include <conio.h>
#include <Windows.h>

// Engine

#include "LAL.hpp"



namespace OSPlatform
{
	CompileTime DWORD Console_Cursor_MinSize = 1;

	CompileTime bool Console_Cursor_NotVisible = 0;

	CompileTime WORD Console_WhiteCell = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

	namespace StdHandle
	{
		CompileTime DWORD Input  = STD_INPUT_HANDLE;
		CompileTime DWORD Output = STD_OUTPUT_HANDLE;
		CompileTime DWORD Error  = STD_ERROR_HANDLE;

		HANDLE Invalid();
	}



	// Enums

	enum class EKeyCode
	{
		None        = 0x00,
		Arrow_Up    = VK_UP,
		Arrow_Down  = VK_DOWN,
		Arrow_Left  = VK_LEFT,
		Arrow_Right = VK_RIGHT,
		Enter       = VK_RETURN,
		Tab         = VK_TAB,
	};



	// Functions

	ptr<FILE> StdInput (void);
	ptr<FILE> StdOutput(void);
	ptr<FILE> StdError (void);

	bool Bind_IOBufferTo_Console(void);

	bool RequestConsole(void);

	bool Unbind_IOBufferTo_Console(void);

	bool GetKeySignal(EKeyCode _key);
}

