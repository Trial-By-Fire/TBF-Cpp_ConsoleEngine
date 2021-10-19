#pragma once



// Windows Library

#include <conio.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Engine

#include "LAL.hpp"



namespace OSPlatform
{
	constexpr DWORD Console_Cursor_MinSize    = 1;
	constexpr bool  Console_Cursor_NotVisible = 0;

	

	namespace StdHandle
	{
		constexpr DWORD Input  = STD_INPUT_HANDLE;
		constexpr DWORD Output = STD_OUTPUT_HANDLE;
		constexpr DWORD Error  = STD_ERROR_HANDLE;

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

	enum class CAttribute : WORD
	{
		FG_Red       = FOREGROUND_RED,
		FG_Green     = FOREGROUND_GREEN,
		FG_Blue      = FOREGROUND_BLUE,
		FG_Intensity = FOREGROUND_INTENSITY,
		BG_Red       = BACKGROUND_RED,
		BG_Green     = BACKGROUND_GREEN,
		BG_Blue      = BACKGROUND_BLUE,
		BG_Intensity = BACKGROUND_INTENSITY,

		SpecifyBitmaskable
	};


	using CAttributeField = Bitfield<CAttribute, WORD>;

	constexpr 
	CAttributeField Console_WhiteCell(CAttribute::FG_Red, CAttribute::FG_Green, CAttribute::FG_Blue, CAttribute::FG_Intensity);

	struct ConsoleCell
	{
		ConsoleCell() = default;

		ConsoleCell(WChar _char, CAttributeField _attributes) :
			Char(_char), Attributes(_attributes)
		{}

		operator CHAR_INFO()
		{
			CHAR_INFO result = { Char, Attributes };

			return result;
		}

		WChar           Char;
		CAttributeField Attributes;
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

